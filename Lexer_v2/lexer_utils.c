#include "lexer.h"
#include <ctype.h>

/**
 * @brief Get current character without advancing position
 * @param lexer The lexer instance
 * @return Current character or '\0' if at end
 */
char	lexer_peek(t_lexer *lexer)
{
	if (!lexer || lexer->pos >= lexer->length)
		return ('\0');
	return (lexer->input[lexer->pos]);
}

/**
 * @brief Advance lexer position by one character
 * @param lexer The lexer instance
 */
void	lexer_advance(t_lexer *lexer)
{
	if (!lexer || lexer->pos >= lexer->length)
		return ;
	if (lexer->input[lexer->pos] == '\n')
	{
		lexer->current_pos.line++;
		lexer->current_pos.column = 1;
	}
	else
		lexer->current_pos.column++;
	lexer->pos++;
}

/**
 * @brief Skip whitespace characters
 * @param lexer The lexer instance
 */
void	lexer_skip_whitespace(t_lexer *lexer)
{
	char	c;

	if (!lexer)
		return ;
	while (lexer->pos < lexer->length)
	{
		c = lexer_peek(lexer);
		if (!isspace(c))
			break ;
		lexer_advance(lexer);
	}
}

/**
 * @brief Check if character is a special shell character
 * @param c The character to check
 * @return true if special character, false otherwise
 */
bool	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '\'' || 
			c == '"' || c == '$' || isspace(c));
}
