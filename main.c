#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "lexer.h"
#include "parse.h"
#include "ast.h"
#include "executor.h"
int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break;
		if (*input)
			add_history(input);
		t_token *tokens = lexer(input);
		t_ast_node  *ast = parse_line(tokens);
		if(ast)
		{
			exec_ast(ast);
		}
		print_tokens(tokens);
		print_ast(ast,3);
		free_tokens(tokens);
		free(input);
	}
	return (0);
}

