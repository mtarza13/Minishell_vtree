/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-23 10:19:53 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-23 10:19:53 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *create_token(t_token_type type, char *value) {
    t_token *token = malloc(sizeof(t_token));
    if (!token) return NULL;
    token->type = type;
    token->value = value ? ft_strdup(value) : NULL;
    token->next = NULL;
    return token;
}

void add_token(t_token **tokens, t_token *new_token) {
    if (!*tokens) { *tokens = new_token; return; }
    t_token *current = *tokens;
    while (current->next) current = current->next;
    current->next = new_token;
}

t_token *tokenize(char *input) {
    if (!input) return NULL;
    t_token *tokens = NULL;
    int i = 0;
    
    while (input[i]) {
        while (input[i] == ' ' || input[i] == '\t') i++;
        if (!input[i]) break;
        
        if (input[i] == '|') {
            add_token(&tokens, create_token(TOKEN_PIPE, "|")); 
            i++;
        } else if (input[i] == '<') {
            if (input[i+1] == '<') {
                add_token(&tokens, create_token(TOKEN_HEREDOC, "<<")); 
                i += 2;
            } else {
                add_token(&tokens, create_token(TOKEN_REDIR_IN, "<")); 
                i++;
            }
        } else if (input[i] == '>') {
            if (input[i+1] == '>') {
                add_token(&tokens, create_token(TOKEN_REDIR_APPEND, ">>")); 
                i += 2;
            } else {
                add_token(&tokens, create_token(TOKEN_REDIR_OUT, ">")); 
                i++;
            }
        } else if (input[i] == '(') {
            add_token(&tokens, create_token(TOKEN_PAREN_OPEN, "(")); 
            i++;
        } else if (input[i] == ')') {
            add_token(&tokens, create_token(TOKEN_PAREN_CLOSE, ")")); 
            i++;
        } else {
            int start = i;
            int in_single_quote = 0, in_double_quote = 0;
            
            while (input[i]) {
                if (!in_single_quote && !in_double_quote &&
                    (input[i] == ' ' || input[i] == '\t' ||
                     input[i] == '|' || input[i] == '<' || input[i] == '>' ||
                     input[i] == '(' || input[i] == ')'))
                    break;
                    
                if (input[i] == '\'' && !in_double_quote)
                    in_single_quote = !in_single_quote;
                else if (input[i] == '"' && !in_single_quote)
                    in_double_quote = !in_double_quote;
                else if (input[i] == '\\' && input[i + 1])
                    i++; // Skip escaped character
                i++;
            }
            
            int len = i - start;
            if (len > 0) {
                char *word = malloc(len + 1);
                if (!word) break;
                ft_strncpy(word, input + start, len);
                word[len] = '\0';
                add_token(&tokens, create_token(TOKEN_WORD, word));
                free(word);
            }
        }
    }
    return tokens;
}

void free_tokens(t_token *tokens) {
    while (tokens) {
        t_token *tmp = tokens;
        tokens = tokens->next;
        if (tmp->value) free(tmp->value);
        free(tmp);
    }
}