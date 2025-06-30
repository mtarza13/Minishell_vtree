#include "parser.h"
#include <stdlib.h>
#include <string.h>

int count_args(t_token *start)
{
    int count = 0;
    while (start && start->type == WORD)
    {
        count++;
        start = start->next;
    }
    return count;
}

t_cmd *parse_tokens(t_token *tokens)
{
    t_cmd *head = NULL;
    t_cmd *last = NULL;

    while (tokens)
    {
        t_cmd *cmd = malloc(sizeof(t_cmd));
        int argc = count_args(tokens);
        cmd->args = malloc(sizeof(char *) * (argc + 1));
        int i = 0;
        while (tokens && tokens->type == WORD)
        {
            cmd->args[i++] = strdup(tokens->value);
            tokens = tokens->next;
        }
        cmd->args[i] = NULL;
        cmd->next = NULL;

        if (tokens && tokens->type == PIPE)
            tokens = tokens->next;

        if (!head)
            head = cmd;
        else
            last->next = cmd;
        last = cmd;
    }
    return head;
}

void free_cmd_data(t_cmd *cmd)
{
    while (cmd)
    {
        t_cmd *tmp = cmd;
        for (int i = 0; cmd->args[i]; i++)
            free(cmd->args[i]);
        free(cmd->args);
        cmd = cmd->next;
        free(tmp);
    }
}
