/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:01:54 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 00:54:09 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_flag(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	if (s[i] != '-')
		return (1);
	while (s[++i])
	{
		if (s[i] != 'n')
			return (1);
	}
	return (0);
}

int	builtin_echo(char **args)
{
	int		i;
	int		newline;
	char	*string;

	i = 1;
	newline = is_valid_flag(args[i]);
	if (!newline)
		while (!is_valid_flag(args[++i]))
			;
	string = ft_strdup("");
	while (args[i])
	{
		string = ft_strjoin(string, args[i]);
		if (args[i + 1])
			string = ft_strjoin(string, " ");
		i++;
	}
	if (newline)
		ft_putstr_fd(ft_strjoin(string, "\n"), 1);
	else
		ft_putstr_fd(string, 1);
	return (0);
}
