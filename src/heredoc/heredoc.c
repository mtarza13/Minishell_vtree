/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:24:24 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/03 22:58:47 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_heredoc_filename(void)
{
	int		fd;
	int		i;
	char	*filename;
	char	*base;

	base = ft_strdup("0123456789abcdefghijklmnopqrstuvwxyz");
	fd = open("/dev/random", O_RDONLY);
	filename = ft_malloc(21, 1337);
	read(fd, filename, 20);
	filename[0] = '.';
	i = 0;
	while (++i < 20)
		filename[i] = base[(unsigned char)filename[i] % 36];
	filename[20] = '\0';
	close(fd);
	filename = ft_strjoin("/tmp/", filename);
	return (filename);
}

void	heredoc_handle(char *file, char *dlimit, int expand, t_data *data)
{
	int (fd);
	char *(input), **(args);
	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			close(fd);
			(ft_malloc(0, 0), exit(131));
		}
		if (!ft_strcmp(input, dlimit))
			break ;
		if (!expand)
		{
			args = ft_malloc(sizeof(char *) * 2, 1337);
			args[0] = input;
			args[1] = NULL;
			input = expand_arg_list(args, data);
		}
		write(fd, ft_strjoin(input, "\n"), ft_strlen(input) + 1);
	}
	close(fd);
	(ft_malloc(0, 0), exit(0));
}

int	heredoc_sig_status(char *dlimit, int status)
{
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
		{
			ft_printf("\n");
			return (0);
		}
		else if (WEXITSTATUS(status) == 131)
		{
			ft_printf("%s (wanted `%s')\n", \
			ft_strjoin("minishell: warning: here-document " \
			, "delimited by end-of-file"), dlimit);
			return (1);
		}
	}	
	return (1);
}

int	feed_heredoc(char *file, char *dlimit, int expand, t_data *data)
{
	int	pid;
	int	status;

	if (expand)
		dlimit = remove_quotes_advanced(dlimit);
	signals_heredoc();
	pid = fork();
	if (!pid)
	{
		signals_heredoc_child();
		heredoc_handle(file, dlimit, expand, data);
	}
	waitpid(pid, &status, 0);
	if (!heredoc_sig_status(dlimit, status))
		return (0);
	return (1);
}

int	heredoc_check(t_token *token, t_data *data)
{
	char	*dlimit;

	while (token)
	{
		if (token->type == TOKEN_HEREDOC)
		{
			dlimit = token->next->value;
			token->next->value = get_heredoc_filename();
			if (!feed_heredoc(token->next->value, dlimit, \
				(ft_strchr(dlimit, '\'') || ft_strchr(dlimit, '"')), data))
				return (0);
		}
		token = token->next;
	}
	return (1);
}
