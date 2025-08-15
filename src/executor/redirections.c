/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 22:54:05 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/03 15:55:41 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_input_redirection(t_redir *redir, t_data *data, int f)
{
	char	**expanded;
	int		fd;
	int		word_count;

	word_count = 0;
	expanded = expand_arg_array(&redir->target, data);
	if (!expanded)
		return (ft_printf("minishell: ambiguous redirect\n"), 0);
	while (expanded[word_count])
		word_count++;
	if (word_count != 1)
		return (ft_printf("minishell: ambiguous redirect\n"), 0);
	fd = open(expanded[0], O_RDONLY);
	if (fd == -1)
		return (ft_printf("minishell: %s: No such file or directory\n",
				expanded[0]), 0);
	if (f)
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

int	handle_output_redirection(t_redir *redir, t_data *data, int f)
{
	char	**expanded;
	int		fd;
	int		flags;
	int		word_count;

	word_count = 0;
	expanded = expand_arg_array(&redir->target, data);
	if (!expanded)
		return (ft_printf("minishell: ambiguous redirect\n"), 0);
	while (expanded[word_count])
		word_count++;
	if (word_count != 1)
		return (ft_printf("minishell: ambiguous redirect\n"), 0);
	if (redir->type == TOKEN_REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(expanded[0], flags, 0644);
	if (fd == -1)
		return (ft_printf("minishell: %s: No such file or directory\n",
				expanded[0]), 0);
	if (f)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	setup_redirections(t_redir *redirs, t_data *data)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
		{
			if (!handle_input_redirection(current, data, 1))
				return (0);
		}
		else if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND)
		{
			if (!handle_output_redirection(current, data, 1))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_is_redir(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}
