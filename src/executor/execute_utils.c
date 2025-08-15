/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 10:09:52 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/04 03:40:57 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	ft_strchr_ex(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (0);
		i++;
	}
	return (1);
}

static char	*ft_strjoin_ex(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	char	*dest;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1) + 1;
	s2_len = ft_strlen(s2);
	dest = ft_malloc((s1_len + s2_len) * (sizeof(char) + 1), 1337);
	i = 0;
	while (s1[i] && i < s1_len - 1)
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i++] = '/';
	dest[i] = '\0';
	ft_strncat(dest, s2, s2_len + s1_len + 1);
	return (dest);
}

static void	error_n_exit(int perr, char *cmd, int err)
{
	if (perr == 0)
		ft_printf("minishell: %s: Is a directory\n", cmd);
	else if (perr == 1)
		ft_printf("minishell: %s: Permission denied\n", cmd);
	else if (perr == 2)
		ft_printf("minishell: %s: No such file or directory\n", cmd);
	else if (perr == 3)
		ft_printf("minishell: %s: command not found\n", cmd);
	ft_malloc(0, 0);
	exit(err);
}

static char	*get_cmd_path(char *path, char *cmd)
{
	char		**arr;
	char		*c_path;
	struct stat	b;
	int			i;

	i = -1;
	arr = ft_split(path, ':');
	while (arr[++i])
	{
		c_path = ft_strjoin_ex(arr[i], cmd);
		stat(c_path, &b);
		if (!access(c_path, F_OK) && !S_ISDIR(b.st_mode))
		{
			if (!access(c_path, X_OK))
				return (c_path);
			else
				error_n_exit(1, c_path, PERMISSION_DENIED);
		}
	}
	error_n_exit(3, cmd, COMMAND_NOT_FOUND);
	return (NULL);
}

char	*filename(char *cmd, t_data *data)
{
	char		*path;
	struct stat	b;

	if (!ft_strchr_ex(cmd, '/'))
	{
		stat(cmd, &b);
		if (S_ISDIR(b.st_mode))
			error_n_exit(0, cmd, PERMISSION_DENIED);
		if (!access(cmd, F_OK))
		{
			if (!access(cmd, X_OK))
				return (cmd);
			else
				error_n_exit(1, cmd, PERMISSION_DENIED);
		}
		else
			error_n_exit(2, cmd, COMMAND_NOT_FOUND);
		return (cmd);
	}
	path = get_env_value("PATH", data);
	if (!path)
		error_n_exit(2, cmd, COMMAND_NOT_FOUND);
	return (get_cmd_path(path, cmd));
}
