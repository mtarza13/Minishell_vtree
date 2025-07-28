#include "../include/minishell.h"

/*
** Forward declarations for static functions. This allows us to call them
** before they are defined.
*/
static t_ast	*parse_simple_command(t_token **tokens);
static t_ast	*creat_pipe_node(t_ast *left, t_ast *right);
static int		count_command_arg(t_token *token);

/*
** Counts the number of WORD tokens in a single simple command.
** It stops at a pipe or the end of the list.
*/
static int	count_command_arg(t_token *token)
{
	int	i;

	i = 0;
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_WORD)
			i++;
		token = token->next;
	}
	return (i);
}

/*
** Creates a NODE_COMMAND. This is the lowest level of the parser.
** It handles one command and its arguments (e.g., "ls -l").
*/
static t_ast	*parse_simple_command(t_token **tokens)
{
	t_ast	*command;
	int		arg_count;
	int		i;

	if (!*tokens || (*tokens)->type == TOKEN_PIPE)
		return (NULL);
	arg_count = count_command_arg(*tokens);
	if (arg_count == 0)
		return (NULL);
	command = malloc(sizeof(t_ast));
	if (!command)
		return (NULL);
	command->type = NODE_COMMAND;
	command->left = NULL;
	command->right = NULL;
	command->argv = malloc(sizeof(char *) * (arg_count + 1));
	if (!command->argv)
	{
		free(command);
		return (NULL);
	}
	i = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			command->argv[i] = ft_strdup((*tokens)->value);
			i++;
		}
		*tokens = (*tokens)->next;
	}
	command->argv[i] = NULL;
	return (command);
}

/*
** Creates a NODE_PIPE to connect two branches of the AST.
*/
static t_ast	*creat_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*pip_node;

	pip_node = malloc(sizeof(t_ast));
	if (!pip_node)
		return (NULL);
	pip_node->type = NODE_PIPE;
	pip_node->left = left;
	pip_node->right = right;
	pip_node->argv = NULL;
	return (pip_node);
}

/*
** This is the main parsing function. It handles the pipeline grammar
** using recursion.
*/
t_ast	*parse(t_token **tokens)
{
	t_ast	*left_node;
	t_ast	*right_node;

	if (!tokens || !*tokens)
		return (NULL);
	left_node = parse_simple_command(tokens);
	if (!left_node)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		if (!*tokens)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			free(left_node);
			return (NULL);
		}
		right_node = parse(tokens);
		if (!right_node)
		{
			free(left_node);
			return (NULL);
		}
		return (creat_pipe_node(left_node, right_node));
	}
	return (left_node);
}
