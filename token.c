#include "token.h"
/*
==============================================================================
 token.c - Token linked list management functions
==============================================================================

 Description:
 This file implements the creation, insertion, printing, and freeing of
 Token structures used in lexical analysis for minishell.

 Functions:
 - Token *creat_token(char *value, tokentype type)
   Allocates and initializes a new Token with the given value and type.

 - void add_token(Token **head, Token *new_token)
   Appends a new token to the end of the token linked list.

 - void print_token(Token *head)
   Prints all tokens in the list with their types and values.

 - void free_token(Token *head)
   Frees all memory associated with the token linked list.

 Notes:
 - strdup is used to duplicate token strings for safe storage.
 - The linked list allows sequential token traversal for parsing.

==============================================================================
*/

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
	while(tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
		}
		
}
void print_token(Token *head)
{ 
	while(head)
	{
		printf("[Token :%d => %s]\n", head->type, head->value);
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

