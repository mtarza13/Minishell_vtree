/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:27:37 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/02 19:58:48 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast), 1337);
	node->type = NODE_PIPE;
	node->args = NULL;
	node->arg_count = 0;
	node->redirs = NULL;
	node->left = left;
	node->right = right;
	return (node);
}

t_ast	*parse_pipeline(t_token **tokens, t_data *data)
{
	t_ast	*left;
	t_ast	*right;

	left = pars_node(tokens, data);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		data->pipes = true;
		if (!*tokens || (*tokens)->type == TOKEN_PIPE)
		{
			ft_printf("minishell: syntax error near unexpected token `|'\n");
			data->status = 2;
			return (free_ast(left), NULL);
		}
		right = pars_node(tokens, data);
		if (!right)
			return (free_ast(left), NULL);
		left = create_pipe_node(left, right);
	}
	return (left);
}
