/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:05:36 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/02 20:03:08 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_env_node(t_env *env, char *key, char *value)
{
	int (v_len);
	v_len = ft_strlen(key);
	while (env)
	{
		if (!ft_strncmp(env->key, key, v_len))
		{
			env->value = value;
			return ;
		}
		env = env->next;
	}
}

int	is_env_char(char c)
{
	return ((c >= 'a' && c <= 'z') || \
			(c >= 'A' && c <= 'Z') || \
			(c >= '0' && c <= '9') || \
			c == '_');
}

int	is_valid_key(char *key, t_data *data)
{
	t_env	*tmp;
	size_t	v_len;

	if (!key)
		return (0);
	tmp = data->env;
	v_len = ft_strlen(key);
	tmp = data->env;
	while (tmp)
	{
		if (ft_strlen(tmp->key) == v_len && !ft_strncmp(tmp->key, key, v_len))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
