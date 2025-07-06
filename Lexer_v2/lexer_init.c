#include "lexer.h"
#include <string.h>

/**
 * @brief Initialize lexer with input string
 * @param input The input string to tokenize
 * @return Initialized lexer or NULL on failure
 * this is first step 
 */
t_lexer	*lexer_init(const char *input)
{
	t_lexer	*lexer;

	if (!input)
		return (NULL);
	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = strdup(input);
	if (!lexer->input)
	{
		free(lexer);
		return (NULL);
	}
	lexer->pos = 0;
	lexer->length = strlen(input);
	lexer->current_pos.line = 1;
	lexer->current_pos.column = 1;
	lexer->has_error = false;
	lexer->error_msg = NULL;
	return (lexer);
}

/**
 * @brief Create new token with given parameters
 * @param type Type of the token
 * @param value Value string of the token
 * @param pos Position information
 * @return Created token or NULL on failure
 */
t_token	*create_token(t_token_type type, const char *value, t_position pos)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = NULL;
	if (value)
	{
		token->value = strdup(value);
		if (!token->value)
		{
			free(token);
			return (NULL);
		}
	}
	token->position = pos;
	token->next = NULL;
	return (token);
}

/**
 * @brief Free lexer and all associated memory
 * @param lexer The lexer to free
 */
void	lexer_free(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->input)
		free(lexer->input);
	if (lexer->error_msg)
		free(lexer->error_msg);
	free(lexer);
}

/**
 * @brief Free token and its value
 * @param token The token to free
 */
void	token_free(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}
