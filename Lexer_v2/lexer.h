#ifndef LEXER_H
# define LEXER_H

# include "lexer_types.h"
# include "lexer_utils.h"
# include "lexer_tokens.h"

/* Initialization and cleanup functions */
t_lexer		*lexer_init(const char *input);
void		lexer_free(t_lexer *lexer);
t_token		*create_token(t_token_type type, const char *value, t_position pos);
void		token_free(t_token *token);

/* Main tokenization functions */
t_token		*lexer_next_token(t_lexer *lexer);
t_token		*lexer_tokenize(t_lexer *lexer);
void		token_list_free(t_token *tokens);
const char	*token_type_str(t_token_type type);

#endif
