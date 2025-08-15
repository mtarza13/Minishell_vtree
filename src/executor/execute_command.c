/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 10:08:28 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/04 15:09:02 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exit_status(int status)
{
	if ((WTERMSIG(status) + 128) == 131 || WEXITSTATUS(status) == 131)
		ft_printf("\n");
	else if ((WTERMSIG(status) + 128) == 130 || WEXITSTATUS(status) == 130)
		ft_printf("\n");
}

static int	execute_external_command(char **args, t_data *data, t_redir *redirs)
{
	pid_t (pid);
	int (status);
	char **(envp), *(file);
	pid = fork();
	if (pid == 0)
	{
		signals_child();
		if (!setup_redirections(redirs, data))
			(ft_malloc(0, 0), exit(EXIT_FAILURE));
		envp = env_to_array(data);
		file = filename(args[0], data);
		execve(file, args, envp);
		if (errno == 13)
			(ft_printf("minishell: %s: Permission denied\n", args[0]),
				ft_malloc(0, 0), exit(PERMISSION_DENIED));
		ft_malloc(0, 0);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (exit_status(status), WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (exit_status(status), 128 + WTERMSIG(status));
	return (1);
}

static int	execute_builtin_with_redirections(char **args, t_data *data,
		t_redir *redirs)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (!setup_redirections(redirs, data))
	{
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return (1);
	}
	status = execute_builtin(args, data);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	return (status);
}

static int	redir_check(t_data *data, t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND)
		{
			if (!handle_output_redirection(current, data, 0))
				return (EXIT_FAILURE);
		}
		else if (current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_HEREDOC)
		{
			if (!handle_input_redirection(current, data, 0))
				return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int	execute_command(char **args, t_data *data, t_redir *redirs)
{
	if (!args || !args[0])
	{
		if (redirs)
		{
			if (redir_check(data, redirs))
				return (EXIT_FAILURE);
			return (EXIT_SUCCESS);
		}
		return (EXIT_SUCCESS);
	}
	if (is_builtin(args[0]))
		return (execute_builtin_with_redirections(args, data, redirs));
	else
		return (execute_external_command(args, data, redirs));
}
