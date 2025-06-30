#include <stdlib.h>  // Added missing include
#include <string.h>  // Added missing include
#include <stdio.h>   // Added for printf
#include "parcer.h"

t_cmd *creat_new_cmmond()
{
    t_cmd *new = malloc(sizeof(t_cmd));
    if (!new)
        return NULL;  // Check malloc failure
    new->args = NULL;
    new->argc = 0;
    new->argv = NULL;
    new->infile = NULL;
    new->next = NULL;
    new->outfile = NULL;
    new->append = 0;
    return new;
}

void add_arg(t_cmd *cmd , char *word)
{
    t_arg *new = malloc(sizeof(t_arg));
    if (!new)
        return;  // Check malloc failure
    new->value = strdup(word);
    new->next = NULL;
    if(cmd->args == NULL)
    {
        cmd->args = new;
    }
    else
    {
        t_arg *tmp = cmd->args;
        while(tmp->next)
        {
            tmp = tmp->next;
        }
        tmp->next = new;
    }
    cmd->argc++;
}

void finalize_args(t_cmd *cmd)
{
    cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
    if (!cmd->argv)
        return;  // Check malloc failure
    int i = 0;
    t_arg *tmp = cmd->args;
    while (tmp)
    {
        cmd->argv[i++] = strdup(tmp->value);
        t_arg *to_free = tmp;
        tmp = tmp->next;
        free(to_free->value);
        free(to_free);
    }
    cmd->argv[i] = NULL;
    cmd->args = NULL;
}

t_cmd *parse_tokens(Token *tokens)
{
    t_cmd *head = NULL;
    t_cmd *current = NULL;
    t_cmd *last = NULL;

    while (tokens)
    {
        if (!current)
        {
            current = creat_new_cmmond();
            if (!current)
                break;  // Handle malloc failure
            if (!head)
                head = current;
            else
                last->next = current;
            last = current;
        }

        if (tokens->type == token_word)
            add_arg(current, tokens->value);

        else if (tokens->type == token_redir_in)
        {
            if (tokens->next) {
                tokens = tokens->next;
                current->infile = strdup(tokens->value);
            } else {
                printf("Syntax error: missing file after '<'\n");
                break;
            }
        }
        else if (tokens->type == token_redir_out)
        {
            if (tokens->next) {
                tokens = tokens->next;
                current->outfile = strdup(tokens->value);
                current->append = 0;
            } else {
                printf("Syntax error: missing file after '>'\n");
                break;
            }
        }
        else if (tokens->type == token_append)
        {
            if (tokens->next) {
                tokens = tokens->next;
                current->outfile = strdup(tokens->value);
                current->append = 1;
            } else {
                printf("Syntax error: missing file after '>>'\n");
                break;
            }
        }
        else if (tokens->type == token_pip)
            current = NULL;

        tokens = tokens->next;
    }
    return head;
}
