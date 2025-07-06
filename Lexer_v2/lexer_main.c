#include "lexer.h"

/**
 * @brief Get next token from lexer
 * @param lexer The lexer instance
 * @return Next token or NULL on error/EOF
 * tcheck type and skeep 
 */
t_token	*lexer_next_token(t_lexer *lexer)
{
	char	c;

	if (!lexer)
		return (NULL);
	lexer_skip_whitespace(lexer);
	if (lexer->pos >= lexer->length)
		return (create_token(TOKEN_EOF, NULL, lexer->current_pos));
	c = lexer_peek(lexer);
	if (c == '|' || c == '<' || c == '>')
		return (parse_operator(lexer));
	else if (c == '\'')
		return (parse_quoted_string(lexer, '\''));
	else if (c == '"')
		return (parse_quoted_string(lexer, '"'));
	else if (c == '$')
		return (parse_env_var(lexer));
	else
		return (parse_word(lexer));
}

/**
 * @brief Tokenize entire input string
 * @param lexer The lexer instance
 * @return Linked list of tokens or NULL on error
 * this is second step
 */
t_token	*lexer_tokenize(t_lexer *lexer)
{
	t_token	*current;
	t_token	*new_token;	t_token	*head;


	if (!lexer)
		return (NULL);
	head = NULL;
	current = NULL;
	while (lexer->pos < lexer->length)
	{
		new_token = lexer_next_token(lexer);
		if (!new_token)
			break ;
		if (new_token->type == TOKEN_ERROR)
		{
			token_list_free(head);
			return (new_token);
		}
		if (!head)
			head = new_token;
		else
			current->next = new_token;
		current = new_token;
	}
	return (head);
}

/**
 * @brief Free entire token list
 * @param tokens Head of token list to free
 */
void	token_list_free(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		token_free(current);
		current = next;
	}
}

/**
 * @brief Get string representation of token type
 * @param type The token type
 * @return String representation
 */
const char	*token_type_str(t_token_type type)
{
	static const char	*type_names[] = {
		"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND",
		"REDIR_HEREDOC", "ENV_VAR", "EXIT_STATUS", "SINGLE_QUOTE",
		"DOUBLE_QUOTE", "EOF", "ERROR"
	};

	if (type >= TOKEN_WORD && type <= TOKEN_ERROR)
		return (type_names[type]);
	return ("UNKNOWN");
}
