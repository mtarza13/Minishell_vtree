#ifndef PARSE_H
#define PARSE_H

#include "lexer.h"
#include "ast.h"

t_ast_node *parse_line(t_token *tokens);

#endif
