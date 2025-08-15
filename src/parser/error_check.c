/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:27:50 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/02 20:05:46 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	print_syntax_error(char *token_value, t_data *data)
{
	ft_printf("minishell: syntax error near unexpected token `%s'\n",
		token_value);
	data->status = 2;
	return (0);
}

int	validate_syntax(t_token *tokens, t_data *data)
{
	t_token *(current);
	if (!tokens)
		return (1);
	current = tokens;
	if (current->type == TOKEN_PIPE)
		return (print_syntax_error(current->value, data));
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next)
				return (print_syntax_error("newline", data));
			if (current->next->type == TOKEN_PIPE)
				return (print_syntax_error(current->next->value, data));
		}
		if (check_is_redir(current->type))
		{
			if (!current->next)
				return (print_syntax_error("newline", data));
			if (current->next->type != TOKEN_WORD)
				return (print_syntax_error(current->next->value, data));
		}
		current = current->next;
	}
	return (1);
}
