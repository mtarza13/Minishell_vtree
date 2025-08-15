/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:32:06 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/02 20:09:36 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	string_check(char **args, int i, int j, t_data *data)
{
	int (f);
	f = 0;
	if (!ft_strchr(args[i], '='))
		return (add_env_node(&data->env, new_env_node(args[i], NULL)), 0);
	while (args[i][j])
	{		
		if (!is_env_char(args[i][j]))
		{
			if (j != 0 && args[i][j] == '=')
			{
				args[i][j] = '\0';
				if (!get_env_value(args[i], data) && \
					!is_valid_key(args[i], data))
					add_env_node(&data->env, new_env_node(ft_strdup(args[i]), \
					ft_strdup(&args[i][j + 1])));
				else
					update_env(data, args[i], ft_strdup(&args[i][j + 1]));
			}
			else
				f = 1;
			break ;
		}
		j++;
	}
	return (f);
}

static void	print_envp(t_env **env_arr, int count)
{
	int		i;
	char	*string;

	i = -1;
	while (++i < count)
	{
		string = ft_strdup("");
		string = ft_strjoin(string, ft_strjoin("declare -x ", env_arr[i]->key));
		if (env_arr[i]->value)
			string = ft_strjoin(string, ft_strjoin("=\"", \
				ft_strjoin(env_arr[i]->value, "\"")));
		string = ft_strjoin(string, "\n");
		ft_putstr_fd(string, 1);
	}
}

static void	bubble_sort(t_env **env_arr, int count)
{
	t_env *(tmp);
	int (i), j = 0;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_arr[j]->key, env_arr[j + 1]->key) > 0)
			{
				tmp = env_arr[j];
				env_arr[j] = env_arr[j + 1];
				env_arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}	
}

void	print_sorted_env(t_data *data)
{
	t_env *(tmp), **(env_arr);
	int (i), count = 0;
	tmp = data->env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	env_arr = ft_malloc(sizeof(t_env *) * count, 32);
	tmp = data->env;
	i = 0;
	while (tmp)
	{
		env_arr[i++] = tmp;
		tmp = tmp->next;
	}
	bubble_sort(env_arr, count);
	print_envp(env_arr, count);
}

int	builtin_export(char **args, t_data *data)
{
	int (i), j = 0, flag = 0;
	i = 1;
	if (!args[i] || !args[i][0])
		return (print_sorted_env(data), 0);
	while (args[i])
	{
		if (!((args[i][0] >= 'A' && args[i][0] <= 'Z') ||
			(args[i][0] >= 'a' && args[i][0] <= 'z') ||
			(args[i][0] == '_')))
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n", \
				args[i]);
			i++;
			flag = 1;
			continue ;
		}
		flag = string_check(args, i, j, data);
		if (flag)
			ft_printf("minishell: export: `%s': not a valid identifier\n", \
				args[i]);
		i++;
	}
	return (flag);
}
