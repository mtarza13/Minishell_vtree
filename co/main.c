/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-23 10:19:53 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-23 10:19:53 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int minishell_loop(t_shell *shell) {
    char *line;
    t_token *tokens = NULL;
    t_ast *ast = NULL;
    
    while (1) {
        setup_signals();
        line = readline("minishell$ ");
        
        if (!line) {
            printf("exit\n");
            break;
        }
        
        if (*line)
            add_history(line);
            
        tokens = tokenize(line);
        t_token *token_cursor = tokens;
        ast = parse_pipeline(&token_cursor);
        
        if (ast) {
            exec_ast(ast, shell->env);
        }
        
        free_tokens(tokens);
        free_ast(ast);
        free(line);
    }
    return 0;
}

int main(int argc, char **argv, char **envp) {
    (void)argc;
    (void)argv;
    
    t_shell shell;
    shell.env = init_env(envp);
    shell.pwd = getcwd(NULL, 0);
    shell.oldpwd = NULL;
    
    if (!shell.env) {
        fprintf(stderr, "Error: Failed to initialize environment\n");
        return 1;
    }
    
    minishell_loop(&shell);
    
    free_env(shell.env);
    if (shell.pwd) free(shell.pwd);
    if (shell.oldpwd) free(shell.oldpwd);
    
    return 0;
}