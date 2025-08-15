/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:32:17 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/02 20:31:33 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_env(t_data *data)
{
	t_env	*env;

	env = data->env;
	while (env)
	{
		if (env->value)
			ft_putstr_fd(ft_strjoin(env->key, ft_strjoin("=", \
			ft_strjoin(env->value, "\n"))), 1);
		env = env->next;
	}
	return (0);
}
