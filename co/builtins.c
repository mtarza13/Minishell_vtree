/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-23 10:19:53 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-23 10:19:53 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to sort environment variables for export display
static void sort_env_for_export(char **env_array, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (env_array[j] && env_array[j + 1] && 
                ft_strcmp(env_array[j], env_array[j + 1]) > 0) {
                char *temp = env_array[j];
                env_array[j] = env_array[j + 1];
                env_array[j + 1] = temp;
            }
        }
    }
}

// Print environment variable in export format
static void print_export_format(char *env_var) {
    char *equals = ft_strchr(env_var, '=');
    if (equals) {
        *equals = '\0';
        printf("declare -x %s=\"%s\"\n", env_var, equals + 1);
        *equals = '=';
    } else {
        printf("declare -x %s\n", env_var);
    }
}

int builtin_export(char **args, t_env *env) {
    // Case 1: export without arguments - display all exported variables
    if (!args[1]) {
        char **sorted_env = malloc(sizeof(char*) * (env->count + 1));
        if (!sorted_env) return 1;
        
        int j = 0;
        for (int i = 0; i < env->count; i++) {
            if (env->envp[i]) {
                sorted_env[j++] = ft_strdup(env->envp[i]);
            }
        }
        sorted_env[j] = NULL;
        
        sort_env_for_export(sorted_env, j);
        
        for (int i = 0; i < j; i++) {
            if (sorted_env[i]) {
                print_export_format(sorted_env[i]);
            }
        }
        
        ft_free_array(sorted_env);
        return 0;
    }
    
    // Case 2: export with arguments
    int exit_status = 0;
    for (int i = 1; args[i]; i++) {
        char *arg = args[i];
        char *equals = ft_strchr(arg, '=');
        
        if (equals) {
            // Case: export VAR=value
            *equals = '\0';
            char *name = arg;
            char *value = equals + 1;
            
            if (!is_valid_identifier(name)) {
                fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]);
                exit_status = 1;
                *equals = '='; // Restore original string
                continue;
            }
            
            if (!set_env_value(env, name, value)) {
                fprintf(stderr, "minishell: export: failed to set variable\n");
                exit_status = 1;
            }
            
            *equals = '='; // Restore original string
        } else {
            // Case: export VAR (without value)
            if (!is_valid_identifier(arg)) {
                fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
                exit_status = 1;
                continue;
            }
            
            // Check if variable already exists
            char *existing_value = get_env_value(env, arg);
            if (existing_value) {
                // Variable exists, keep its value but mark as exported
                if (!set_env_value(env, arg, existing_value)) {
                    fprintf(stderr, "minishell: export: failed to export variable\n");
                    exit_status = 1;
                }
            } else {
                // Variable doesn't exist, create it without value
                if (!set_env_value(env, arg, NULL)) {
                    fprintf(stderr, "minishell: export: failed to export variable\n");
                    exit_status = 1;
                }
            }
        }
    }
    
    return exit_status;
}

int builtin_unset(char **args, t_env *env) {
    if (!args[1]) {
        fprintf(stderr, "minishell: unset: not enough arguments\n");
        return 1;
    }
    
    int exit_status = 0;
    for (int i = 1; args[i]; i++) {
        if (!is_valid_identifier(args[i])) {
            fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n", args[i]);
            exit_status = 1;
            continue;
        }
        
        unset_env_value(env, args[i]);
    }
    
    return exit_status;
}

int builtin_echo(char **args, t_env *env) {
    (void)env;
    int i = 1;
    int newline = 1;
    
    if (args[1] && ft_strcmp(args[1], "-n") == 0) {
        newline = 0;
        i = 2;
    }
    
    while (args[i]) {
        printf("%s", args[i]);
        if (args[i + 1]) printf(" ");
        i++;
    }
    if (newline) printf("\n");
    return 0;
}

int builtin_pwd(char **args, t_env *env) {
    (void)args;
    (void)env;
    char *pwd = getcwd(NULL, 0);
    if (pwd) {
        printf("%s\n", pwd);
        free(pwd);
        return 0;
    }
    return 1;
}

int builtin_env(char **args, t_env *env) {
    (void)args;
    for (int i = 0; i < env->count; i++) {
        if (env->envp[i] && ft_strchr(env->envp[i], '='))
            printf("%s\n", env->envp[i]);
    }
    return 0;
}

int builtin_cd(char **args, t_env *env) {
    char *path;
    char *old_pwd = getcwd(NULL, 0);
    
    if (!args[1] || ft_strcmp(args[1], "~") == 0) {
        // cd or cd ~ - go to HOME
        path = get_env_value(env, "HOME");
        if (!path) {
            fprintf(stderr, "minishell: cd: HOME not set\n");
            if (old_pwd) free(old_pwd);
            return 1;
        }
    } else if (ft_strcmp(args[1], "-") == 0) {
        // cd - - go to OLDPWD
        path = get_env_value(env, "OLDPWD");
        if (!path) {
            fprintf(stderr, "minishell: cd: OLDPWD not set\n");
            if (old_pwd) free(old_pwd);
            return 1;
        }
        printf("%s\n", path);
    } else {
        path = args[1];
    }
    
    if (chdir(path) == -1) {
        fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
        if (old_pwd) free(old_pwd);
        return 1;
    }
    
    // Update OLDPWD and PWD
    if (old_pwd) {
        set_env_value(env, "OLDPWD", old_pwd);
        free(old_pwd);
    }
    
    char *new_pwd = getcwd(NULL, 0);
    if (new_pwd) {
        set_env_value(env, "PWD", new_pwd);
        free(new_pwd);
    }
    
    return 0;
}

int builtin_exit(char **args, t_env *env) {
    (void)env;
    printf("exit\n");
    if (args[1]) {
        int code = ft_atoi(args[1]);
        exit(code);
    }
    exit(0);
}