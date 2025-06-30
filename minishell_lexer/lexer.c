#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

t_token *new_token(char *value, t_token_type type)
{
    t_token *token = malloc(sizeof(t_token));
    token->value = strdup(value);
    token->type = type;
    token->next = NULL;
    return token;
}

void add_token(t_token **head, t_token *new)
{
    if (!*head)
        *head = new;
    else
    {
        t_token *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

int is_special(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

t_token_type get_token_type(const char *s)
{
    if (!strcmp(s, "|")) return PIPE;
    if (!strcmp(s, "<")) return REDIRECT_IN;
    if (!strcmp(s, ">")) return REDIRECT_OUT;
    if (!strcmp(s, ">>")) return APPEND;
    if (!strcmp(s, "<<")) return HEREDOC;
    return WORD;
}

t_token *tokenize_input(char *input)
{
    t_token *tokens = NULL;
    char *start = input;

    while (*input)
    {
        while (*input == ' ')
            input++;
        if (!*input) break;

        char *end = input;
        if (is_special(*input))
        {
            if (*(input + 1) == *input)
                end += 2;
            else
                end += 1;
        }
        else
        {
            while (*end && *end != ' ' && !is_special(*end))
                end++;
        }

        int len = end - input;
        char *val = strndup(input, len);
        add_token(&tokens, new_token(val, get_token_type(val)));
        free(val);
        input = end;
    }
    return tokens;
}
