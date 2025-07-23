/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-23 10:19:53 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-23 10:19:53 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_ast(t_ast *ast) {
    if (!ast) return;
    if (ast->args) ft_free_array(ast->args);
    t_redir *redir = ast->redirs, *tmp;
    while (redir) {
        tmp = redir;
        redir = redir->next;
        if (tmp->target) free(tmp->target);
        free(tmp);
    }
    free_ast(ast->left);
    free_ast(ast->right);
    free(ast);
}

static int count_args(t_token *tokens) {
    int count = 0;
    t_token *cur = tokens;
    while (cur && cur->type == TOKEN_WORD) {
        count++;
        cur = cur->next;
    }
    return count;
}

static char **copy_args(t_token *tokens, int argc) {
    char **args = malloc(sizeof(char*) * (argc + 1));
    for (int i = 0; i < argc; i++) {
        args[i] = ft_strdup(tokens->value);
        tokens = tokens->next;
    }
    args[argc] = NULL;
    return args;
}

t_ast *parse_simple_command(t_token **tokens) {
    int argc = count_args(*tokens);
    if (argc == 0) return NULL;
    
    t_ast *cmd = malloc(sizeof(t_ast));
    if (!cmd) return NULL;
    
    cmd->type = NODE_COMMAND;
    cmd->args = copy_args(*tokens, argc);
    cmd->arg_count = argc;
    cmd->redirs = NULL;
    cmd->left = cmd->right = NULL;
    
    for (int i = 0; i < argc; i++) 
        (*tokens) = (*tokens)->next;
    
    return cmd;
}

t_ast *parse_pipeline(t_token **tokens) {
    t_ast *left = parse_simple_command(tokens);
    if (!left) return NULL;
    
    while (*tokens && (*tokens)->type == TOKEN_PIPE) {
        *tokens = (*tokens)->next; // Skip pipe
        t_ast *right = parse_simple_command(tokens);
        if (!right) {
            free_ast(left);
            return NULL;
        }
        
        t_ast *pipe = malloc(sizeof(t_ast));
        if (!pipe) {
            free_ast(left);
            free_ast(right);
            return NULL;
        }
        
        pipe->type = NODE_PIPE;
        pipe->args = NULL;
        pipe->arg_count = 0;
        pipe->redirs = NULL;
        pipe->left = left;
        pipe->right = right;
        left = pipe;
    }
    return left;
}