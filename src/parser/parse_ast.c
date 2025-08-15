/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:27:30 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/02 20:05:38 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_word(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (check_is_redir(current->type))
		{
			current = current->next;
			if (current)
				current = current->next;
		}
		else
		{
			count++;
			current = current->next;
		}
	}
	return (count);
}

static t_ast	*init_cmd_node(int argc)
{
	t_ast	*cmd;

	cmd = ft_malloc(sizeof(t_ast), 1337);
	cmd->type = NODE_COMMAND;
	cmd->redirs = NULL;
	cmd->left = NULL;
	cmd->right = NULL;
	cmd->arg_count = argc;
	if (argc > 0)
		cmd->args = ft_malloc(sizeof(char *) * (argc + 1), 69);
	else
		cmd->args = NULL;
	return (cmd);
}

t_ast	*pars_node(t_token **tokens, t_data *data)
{
	t_ast *(cmd);
	int (argc), i = 0;
	argc = count_word(*tokens);
	cmd = init_cmd_node(argc);
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if (check_is_redir((*tokens)->type))
		{
			if (!parse_single_redir(tokens, &cmd->redirs, data))
				return (free_ast(cmd), NULL);
		}
		else if ((*tokens)->type == TOKEN_WORD)
		{
			if (cmd->args)
				cmd->args[i++] = ft_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
		}
		else
			*tokens = (*tokens)->next;
	}
	if (cmd->args)
		cmd->args[i] = NULL;
	if (argc == 0 && !cmd->redirs)
		return (free_ast(cmd), NULL);
	return (cmd);
}
