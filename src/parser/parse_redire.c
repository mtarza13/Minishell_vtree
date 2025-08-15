/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:27:44 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/02 20:05:43 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*creat_redir_node(t_token_type type, char *target, t_data *data)
{
	int		should_expand;
	char	*final_delimiter;
	t_redir	*redir;

	redir = ft_malloc(sizeof(t_redir), 1337);
	redir->type = type;
	redir->target = ft_strdup(target);
	redir->expanded_target = NULL;
	redir->quoted_delimiter = 0;
	redir->fd = -1;
	redir->next = NULL;
	if (type == TOKEN_HEREDOC)
	{
		if (analyze_heredoc_delimiter(target, &final_delimiter, &should_expand,
				data))
			redir->quoted_delimiter = !should_expand;
	}
	return (redir);
}

void	add_redirection(t_redir **redirs, t_redir *new_redir)
{
	t_redir	*current;

	if (!*redirs)
	{
		*redirs = new_redir;
		return ;
	}
	current = *redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

int	parse_single_redir(t_token **tokens, t_redir **redirs, t_data *data)
{
	t_token_type	redir_type;
	t_redir			*new_redir;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		ft_printf("minishell: syntax error near unexpected token\n");
		return (0);
	}
	new_redir = creat_redir_node(redir_type, (*tokens)->value, data);
	if (!new_redir)
		return (0);
	add_redirection(redirs, new_redir);
	*tokens = (*tokens)->next;
	return (1);
}
