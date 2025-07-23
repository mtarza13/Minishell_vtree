/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-23 10:19:53 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-23 10:19:53 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *cmd) {
    return (ft_strcmp(cmd, "echo") == 0 || 
            ft_strcmp(cmd, "cd") == 0 ||
            ft_strcmp(cmd, "pwd") == 0 ||
            ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0 ||
            ft_strcmp(cmd, "env") == 0 ||
            ft_strcmp(cmd, "exit") == 0);
}

int execute_builtin(char **args, t_env *env) {
    if (ft_strcmp(args[0], "echo") == 0)
        return builtin_echo(args, env);
    if (ft_strcmp(args[0], "cd") == 0)
        return builtin_cd(args, env);
    if (ft_strcmp(args[0], "pwd") == 0)
        return builtin_pwd(args, env);
    if (ft_strcmp(args[0], "export") == 0)
        return builtin_export(args, env);
    if (ft_strcmp(args[0], "unset") == 0)
        return builtin_unset(args, env);
    if (ft_strcmp(args[0], "env") == 0)
        return builtin_env(args, env);
    if (ft_strcmp(args[0], "exit") == 0)
        return builtin_exit(args, env);
    return 1;
}

int execute_command(char **args, t_env *env) {
    if (!args || !args[0]) return 0;
    
    if (is_builtin(args[0])) {
        return execute_builtin(args, env);
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        char **envp = env_to_array(env);
        execvp(args[0], args);
        fprintf(stderr, "minishell: %s: %s\n", args[0], strerror(errno));
        ft_free_array(envp);
        exit(errno == EACCES ? PERMISSION_DENIED : COMMAND_NOT_FOUND);
    } else if (pid > 0) {
        int status = 0;
        waitpid(pid, &status, 0);
        env->exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
        return env->exit_status;
    }
    return 1;
}

int exec_ast(t_ast *ast, t_env *env) {
    if (!ast) return 1;
    
    if (ast->type == NODE_COMMAND) {
        char **expanded = expand_args_professional(ast->args, env);
        int status = execute_command(expanded, env);
        ft_free_array(expanded);
        return status;
    } else if (ast->type == NODE_PIPE) {
        int fd[2], status = 0;
        if (pipe(fd) == -1) return 1;
        
        pid_t pid = fork();
        if (pid == 0) {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            status = exec_ast(ast->left, env);
            exit(status);
        } else if (pid > 0) {
            close(fd[1]);
            int saved_stdin = dup(STDIN_FILENO);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            status = exec_ast(ast->right, env);
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin);
            int wstatus;
            waitpid(pid, &wstatus, 0);
            env->exit_status = WIFEXITED(wstatus) ? WEXITSTATUS(wstatus) : 1;
            return status;
        }
    }
    return 1;
}