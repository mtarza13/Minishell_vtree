/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:06:42 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/03 15:14:37 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_strtol(char *str)
{
	long (result), p = 1, n = 0;
	result = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
		{
			n = 1;
			p = 0;
		}
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = (result * 10) + (*str - '0');
		if ((result < 0 && p) || ((result * -1) > 0 && n))
		{
			errno = ERANGE;
			return ;
		}
		str++;
	}
}

static long	ft_exit_input(char *s, long *status)
{
	int (i), f = 0;
	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
		{
			f = 1;
			break ;
		}
		i++;
	}
	ft_strtol(s);
	if ((errno == ERANGE) || f)
	{
		ft_printf("minishell: exit: %s: numeric argument required\n", s);
		*status = 2;
		return (0);
	}
	*status = ft_atoi(s);
	return (0);
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

int	builtin_exit(char **args, t_data *data)
{
	long	status;

	status = 0;
	if (!data->pipes)
		ft_putstr_fd("exit\n", 1);
	if (args[1])
	{
		if (!ft_exit_input(args[1], &status))
		{
			free_data();
			exit(status);
		}
		if (arg_count(args) > 2)
		{
			ft_printf("minishell: exit: too many arguments\n");
			return (1);
		}
	}
	status = data->status;
	free_data();
	exit(status);
}
