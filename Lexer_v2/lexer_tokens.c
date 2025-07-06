#include "lexer.h"
#include <string.h>
#include <ctype.h>

/**
 * @brief Parse word token from current position
 * @param lexer The lexer instance
 * @return Word token or NULL on error
 */
t_token	*parse_word(t_lexer *lexer)
{
	size_t		start;
	size_t		len;
	char		*word;
	t_position	pos;

	if (!lexer)
		return (NULL);
	start = lexer->pos;
	pos = lexer->current_pos;
	while (lexer->pos < lexer->length && !is_special_char(lexer_peek(lexer)))
		lexer_advance(lexer);
	len = lexer->pos - start;
	if (len == 0)
		return (NULL);
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	strncpy(word, &lexer->input[start], len);
	word[len] = '\0';
	return (create_token(TOKEN_WORD, word, pos));
}

/**
 * @brief Parse quoted string token
 * @param lexer The lexer instance
 * @param quote_char The quote character (' or ")
 * @return Quoted string token or NULL on error
 */
t_token	*parse_quoted_string(t_lexer *lexer, char quote_char)
{
	size_t		start;
	size_t		len;
	char		*content;
	t_position	pos;
	t_token_type	type;

	if (!lexer || lexer_peek(lexer) != quote_char)
		return (NULL);
	pos = lexer->current_pos;
	lexer_advance(lexer);
	start = lexer->pos;
	while (lexer->pos < lexer->length && lexer_peek(lexer) != quote_char)
		lexer_advance(lexer);
	if (lexer->pos >= lexer->length)
		return (create_token(TOKEN_ERROR, "Unclosed quote", pos));
	len = lexer->pos - start;
	content = malloc(len + 1);
	if (!content)
		return (NULL);
	strncpy(content, &lexer->input[start], len);
	content[len] = '\0';
	lexer_advance(lexer);
	type = (quote_char == '\'') ? TOKEN_SINGLE_QUOTE : TOKEN_DOUBLE_QUOTE;
	return (create_token(type, content, pos));
}

/**
 * @brief Parse environment variable token
 * @param lexer The lexer instance
 * @return Environment variable token or NULL on error
 */
t_token	*parse_env_var(t_lexer *lexer)
{
	size_t		start;
	size_t		len;
	char		*var_name;
	t_position	pos;

	if (!lexer || lexer_peek(lexer) != '$')
		return (NULL);
	pos = lexer->current_pos;
	lexer_advance(lexer);
	if (lexer_peek(lexer) == '?')
	{
		lexer_advance(lexer);
		return (create_token(TOKEN_EXIT_STATUS, "$?", pos));
	}
	start = lexer->pos;
	while (lexer->pos < lexer->length && 
		   (isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_'))
		lexer_advance(lexer);
	len = lexer->pos - start + 1;
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	var_name[0] = '$';
	strncpy(&var_name[1], &lexer->input[start], len - 1);
	var_name[len] = '\0';
	return (create_token(TOKEN_ENV_VAR, var_name, pos));
}

/**
 * @brief Parse operator tokens (|, <, >, >>, <<)
 * @param lexer The lexer instance
 * @return Operator token or NULL on error
 */
t_token	*parse_operator(t_lexer *lexer)
{
	char		c;
	char		next_c;
	t_position	pos;

	if (!lexer)
		return (NULL);
	c = lexer_peek(lexer);
	pos = lexer->current_pos;
	lexer_advance(lexer);
	if (c == '|')
		return (create_token(TOKEN_PIPE, "|", pos));
	else if (c == '<')
	{
		next_c = lexer_peek(lexer);
		if (next_c == '<')
		{
			lexer_advance(lexer);
			return (create_token(TOKEN_REDIR_HEREDOC, "<<", pos));
		}
		return (create_token(TOKEN_REDIR_IN, "<", pos));
	}
	else if (c == '>')
	{
		next_c = lexer_peek(lexer);
		if (next_c == '>')
		{
			lexer_advance(lexer);
			return (create_token(TOKEN_REDIR_APPEND, ">>", pos));
		}
		return (create_token(TOKEN_REDIR_OUT, ">", pos));
	}
	return (NULL);
}
