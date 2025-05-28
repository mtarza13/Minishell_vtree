#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <strings.h>


typedef enum{
	token_word ,
	token_string,
	token_pip,
	token_redir_in,
	token_redir_out,
	token_append,
	token_herdoc
}tokentype;


typedef struct s_token{
	char *value;
	tokentype type;
	struct s_token *next;
}Token;

Token *creat_token(char *value , tokentype type)
{
	Token *token = malloc(sizeof(Token));
	token->type = type;
	token->value = strdup(value);
	token->next = NULL;
	return token;
} 

void add_token(Token **head,Token *new_token)
{
	if(!*head)
		*head = new_token;
	else{
	Token *tmp = *head;	
	while(tmp)
		tmp = tmp->next;
	tmp = new_token;
		}
		
}
void print_token(Token *head)
{
	while(head)
	{
		printf("[Token :%d => %s\n]", head->type, head->value);
		head = head->next;
	}

}
void free_token(Token *head)
{
	while(head)
	{
		Token *tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}

