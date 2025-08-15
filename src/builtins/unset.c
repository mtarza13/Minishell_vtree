/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:32:33 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/28 20:51:46 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_popnode(t_env **env, char *value)
{
	t_env *(tmp), *(prev);
	size_t (key_len);
	tmp = *env;
	prev = tmp;
	key_len = ft_strlen(tmp->key);
	if (!ft_strncmp(tmp->key, value, key_len))
	{
		*env = tmp->next;
		return ;
	}
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, value, key_len))
		{
			prev->next = tmp->next;
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	builtin_unset(char **args, t_data *data)
{
	int		i;

	i = 1;
	if (args[i])
	{
		while (args[i])
		{
			ft_popnode(&data->env, args[i]);
			i++;
		}
		data->envp = ft_envp(data->env);
	}
	data->status = 0;
	return (data->status);
}
