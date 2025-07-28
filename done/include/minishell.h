#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "../libft/libft.h"

typedef enum s_type_token
{
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HERDOC,
	TOKEN_WORD
}t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
	struct s_token *next;
}t_token;

typedef struct s_redair
{
	t_token_type type;
	char *filename;
	struct s_redair *next;
}t_redair;

typedef struct s_simple_cmd
{
	char **args;
	t_redair **redair;
}t_simple_cmd;

typedef enum s_ast_type
{
	NODE_COMMAND,
	NODE_PIPE 
}t_ast_type;

typedef struct s_ast
{
 	t_ast_type type;
	char **argv;
	struct s_ast *left;
	struct s_ast *right;
	//t_simple_cmd *simple_cmd;
}t_ast;

t_token *tokenaizer(char *input);
//parse
t_ast *parse(t_token **token);

#endif
