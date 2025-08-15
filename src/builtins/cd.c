/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:00:50 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/03 15:20:19 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_cd_path(char **args, t_data *data)
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value("HOME", data);
		if (!path)
		{
			ft_printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	else
		path = args[1];
	return (path);
}

static int	arg_count(char **args)
{
	int	i;

	i = 0;
	if (!args || !args[i])
		return (0);
	while (args[i])
		i++;
	return (i);
}

void	update_env(t_data *data, char *key, char *value)
{
	t_env *(tmp);
	tmp = data->env;
	if (!tmp)
		return (add_env_node(&data->env, new_env_node(ft_strdup(key), \
		ft_strdup(value))), (void)0);
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			tmp->value = ft_strdup(value);
			break ;
		}
		else if (!tmp->next)
		{
			add_env_node(&data->env, new_env_node(ft_strdup(key), \
			ft_strdup(value)));
			break ;
		}
		tmp = tmp->next;
	}
}

static void	getcwd_perror(void)
{
	char	*cd;
	char	*getcwd;
	char	*no_file;

	cd = ft_strdup("cd: error retrieving current directory: ");
	getcwd = ft_strdup("getcwd: cannot access parent directories: ");
	no_file = ft_strdup("No such file or directory");
	ft_printf("%s\n", ft_strjoin(cd, ft_strjoin(getcwd, no_file)));
}

int	builtin_cd(char **args, t_data *data)
{
	char *(path), *(pwd);
	if (arg_count(args) > 2)
		return (ft_printf("minishell: cd: too many arguments\n"), 1);
	if (args[1] && access(args[1], F_OK) == -1)
		return (ft_printf("minishell: cd: %s: %s\n", args[1], \
			strerror(errno)), 1);
	path = get_cd_path(args, data);
	if (!path)
		return (1);
	update_env(data, "OLDPWD", get_env_value("PWD", data));
	if (chdir(path) == -1)
		ft_printf("minishell: cd: %s: Permission denied\n", path);
	pwd = getcwd(NULL, 0);
	if (pwd)
		update_env(data, "PWD", pwd);
	else if (!pwd)
		return (getcwd_perror(), 1);
	free(pwd);
	return (0);
}
