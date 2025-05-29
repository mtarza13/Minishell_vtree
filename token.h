#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <strings.h>

#ifndef TOKEN_H
#define TOKEN_H
typedef enum{
	token_word = 0,
	token_string,
	token_pip,
	token_redir_in,
	token_redir_out,
	token_append,
	token_herdoc,
	token_single_quote,
	token_double_quote,
	
}tokentype;


typedef struct s_token{
	char *value;
	tokentype type;
	struct s_token *next;
}Token;

void lexer(char *line);




Token *creat_token(char *value , tokentype type);
void add_token(Token **head,Token *new_token);
void print_token(Token *head);
void free_token(Token *head);



#endif
