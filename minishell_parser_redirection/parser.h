#ifndef PARSER_H
#define PARSER_H

typedef struct s_cmd
{
    char **args;
    char *infile;
    char *outfile;
    int append;
    struct s_cmd *next;
} t_cmd;

#include "token.h"

t_cmd *parse_tokens(t_token *tokens);
void free_cmd_data(t_cmd *cmd);

#endif
