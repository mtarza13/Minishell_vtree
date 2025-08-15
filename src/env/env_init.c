/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:22:36 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/30 04:24:50 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_envp(t_env *env)
{
	int		i;
	t_env	*tmp;
	char	**envp;

	i = 0;
	tmp = env;
	envp = (char **)ft_malloc(sizeof(char *) * list_len(env), 43);
	while (tmp)
	{
		envp[i] = (char *)ft_malloc(sizeof(char) * (ft_strlen(tmp->key)
					+ ft_strlen(tmp->value) + 2), 65);
		ft_strncpy(envp[i], tmp->key, ft_strlen(tmp->key) + 1);
		ft_strncat(envp[i], "=", ft_strlen(tmp->key) + 2);
		ft_strncat(envp[i], tmp->value, ft_strlen(tmp->value)
			+ ft_strlen(tmp->key) + 2);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	add_env_node(t_env **env, t_env *new)
{
	t_env	*temp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	temp = *env;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_env	*new_env_node(char *key, char *value)
{
	t_env	*new;

	new = ft_malloc(sizeof(t_env), -42);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

void	parse_env_var(t_env **env, char *env_str)
{
	char	*key;
	char	*value;
	char	*equals;
	t_env	*new;

	equals = ft_strchr(env_str, '=');
	if (!equals)
	{
		key = ft_strdup(env_str);
		value = NULL;
	}
	else
	{
		key = ft_strndup(env_str, equals - env_str);
		value = ft_strdup(equals + 1);
	}
	new = new_env_node(key, value);
	if (new)
		add_env_node(env, new);
}

t_env	*init_env(char **env)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		parse_env_var(&env_list, env[i]);
		i++;
	}
	return (env_list);
}
