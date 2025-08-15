/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:33:21 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/02 20:32:55 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(char **args, t_data *data)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, data));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(data));
	if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, data));
	if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, data));
	if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(data));
	if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args, data));
	return (1);
}
