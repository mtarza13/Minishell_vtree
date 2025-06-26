#ifndef PARSER_H
#define PARSER_H


#include "token.h"


typedef struct s_arg{

	char *value;
	struct s_arg *next;
}t_arg;

typedef struct s_cmd
{
	t_arg	*args;
	int 	argc;
	char **argv;
	char *infile;
	char *outfile;
	int append;
	struct s_cmd *next;
}t_cmd;


t_cmd *parse_tokens(Token *tokens);
void finalize_args(t_cmd *cmd);
void print_cmd(t_cmd *cmd);
void free_cmd(t_cmd *cmd);



#endif
