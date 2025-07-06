#ifndef LEXER_UTILS_H
# define LEXER_UTILS_H

# include "lexer_types.h"

/* Utility functions for lexer navigation */
char	lexer_peek(t_lexer *lexer);
void	lexer_advance(t_lexer *lexer);
void	lexer_skip_whitespace(t_lexer *lexer);
bool	is_special_char(char c);

#endif
