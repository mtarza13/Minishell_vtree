/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-11 03:15:00 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-11 03:15:00 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env(char **envp)
{
	int	count = 0;
	if (!envp) return (0);
	while (envp[count]) count++;
	return (count);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		count, i;

	env = malloc(sizeof(t_env));
	if (!env) return (NULL);
	count = count_env(envp);
	env->capacity = count + 50;
	env->envp = malloc(sizeof(char *) * env->capacity);
	if (!env->envp) { free(env); return (NULL); }
	
	for (i = 0; i < count; i++)
		env->envp[i] = ft_strdup(envp[i]);
	while (i < env->capacity)
		env->envp[i++] = NULL;
	env->count = count;
	env->exit_status = 0;
	return (env);
}

void	free_env(t_env *env)
{
	if (!env) return;
	ft_free_array(env->envp);
	free(env);
}

char	*get_env_value(t_env *env, char *name)
{
	int len, i;
	if (!env || !name) return (NULL);
	len = ft_strlen(name);
	for (i = 0; i < env->count; i++)
		if (env->envp[i] && ft_strncmp(env->envp[i], name, len) == 0 && env->envp[i][len] == '=')
			return (env->envp[i] + len + 1);
	return (NULL);
}

int	set_env_value(t_env *env, char *name, char *value)
{
	int len, i;
	char *new_var;
	if (!env || !name) return (0);
	len = ft_strlen(name);
	
	// Find existing
	for (i = 0; i < env->count; i++) {
		if (env->envp[i] && ft_strncmp(env->envp[i], name, len) == 0 && 
			(env->envp[i][len] == '=' || env->envp[i][len] == '\0')) {
			free(env->envp[i]);
			if (value) {
				new_var = ft_strjoin(ft_strdup(name), ft_strdup("="));
				env->envp[i] = ft_strjoin(new_var, ft_strdup(value));
			} else
				env->envp[i] = ft_strdup(name);
			return (1);
		}
	}
	
	// Add new
	if (env->count >= env->capacity - 1) {
		env->capacity *= 2;
		env->envp = realloc(env->envp, sizeof(char*) * env->capacity);
		if (!env->envp) return (0);
		for (i = env->count; i < env->capacity; i++) env->envp[i] = NULL;
	}
	
	if (value) {
		new_var = ft_strjoin(ft_strdup(name), ft_strdup("="));
		env->envp[env->count] = ft_strjoin(new_var, ft_strdup(value));
	} else
		env->envp[env->count] = ft_strdup(name);
	env->count++;
	return (1);
}

int	unset_env_value(t_env *env, char *name)
{
	int len, i;
	if (!env || !name) return (0);
	len = ft_strlen(name);
	for (i = 0; i < env->count; i++) {
		if (env->envp[i] && ft_strncmp(env->envp[i], name, len) == 0 && 
			(env->envp[i][len] == '=' || env->envp[i][len] == '\0')) {
			free(env->envp[i]);
			while (i < env->count - 1) {
				env->envp[i] = env->envp[i + 1];
				i++;
			}
			env->envp[i] = NULL;
			env->count--;
			return (1);
		}
	}
	return (0);
}

char	**env_to_array(t_env *env)
{
	char **array;
	int i, j;
	if (!env) return (NULL);
	array = malloc(sizeof(char*) * (env->count + 1));
	if (!array) return (NULL);
	for (i = 0, j = 0; i < env->count; i++)
		if (env->envp[i] && ft_strchr(env->envp[i], '='))
			array[j++] = ft_strdup(env->envp[i]);
	array[j] = NULL;
	return (array);
}
