#ifndef LEXER_TOKENS_H
# define LEXER_TOKENS_H

# include "lexer_types.h"

/* Token parsing functions */
t_token	*parse_word(t_lexer *lexer);
t_token	*parse_quoted_string(t_lexer *lexer, char quote_char);
t_token	*parse_env_var(t_lexer *lexer);
t_token	*parse_operator(t_lexer *lexer);

#endif
