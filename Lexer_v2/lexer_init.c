#include "lexer_type.h"
#include <string.h>


/**
* @breif Initialize Lexer with input string
* @param input The input string to tokenize
* @patram Initialized lexer or NULL on failure
*/
t_lexer *lexer_init(const char *input)
{
	t_lexer *lexer;
	if(!input)
		return(NULL);
	lexer = malloc(sizeof(t_lexer));
	if(!lexer)
		return(NULL);
	lexer->input = strdup(input);
	if(!lexer->input)
	{
		free(lexer);
	}
	lexer->pos = 0;
	lexer->length= strlen(input);
	lexer->current_pos.line = 1;
	lexer->current_pos.column = 1;
	lexer->has_errors = false;
	lexer->error_msg = NULL;
	return(lexer);
}
/**
* @brief creat new token with given parameters
* @param type Type of the token 
* @param value  Value string of the token
* @param pos Position information
* @return created Token or NULL on failer 
*/

t_token *creat_token(t_token_type type , const char *value , t_position pos)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if(!token)
		return(NULL);
	token->type = type;
	token->next = NULL;
	if(value)
	{
		token->value = strdup(value);
		if(!token->value)
		{
			free(token);
			return(NULL);
		}
	}
	token->position = pos;
	return(token);
}
/**
* @brief free lexer and all associated memory
* @parap lexer Thle lexer to free
*/

void lexer_free(t_lexer *lexer)
{
	if(!lexer)
		return;
	if(lexer->input)
		free(lexer->input);
	if(lexer->error_msg)
			free(lexer->error_msg);
	free(lexer);
}
/**
* @brief free token and its vaule
* @param token The token to be free
*/

void token_free(t_token *token)
{
	if(!token)
		free(token);
	if(token->value)
		free(token->value);
	free(token);
}
