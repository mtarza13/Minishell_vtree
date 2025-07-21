/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-11 03:15:00 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-11 03:15:00 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	start = *i;
	
	// Handle $?
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	
	// Handle $0, $1, etc.
	if (ft_isdigit(str[*i]))
	{
		(*i)++;
		name = malloc(2);
		name[0] = str[start];
		name[1] = '\0';
		return (name);
	}
	
	// Handle regular variables
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (NULL);
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	ft_strncpy(name, str + start, len);
	name[len] = '\0';
	return (name);
}

static char	*expand_var(char *var_name, t_env *env)
{
	char	*value;
	char	exit_str[12];

	if (ft_strcmp(var_name, "?") == 0)
	{
		snprintf(exit_str, sizeof(exit_str), "%d", env->exit_status);
		return (ft_strdup(exit_str));
	}
	if (ft_strcmp(var_name, "0") == 0)
		return (ft_strdup("minishell"));
	if (ft_isdigit(var_name[0]))
		return (ft_strdup(""));
	value = get_env_value(env, var_name);
	return (value ? ft_strdup(value) : ft_strdup(""));
}

char	*expand_variables(char *str, t_env *env)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			temp = malloc(2);
			temp[0] = str[i];
			temp[1] = '\0';
			result = ft_strjoin(result, temp);
			i++;
		}
		else if (str[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			temp = malloc(2);
			temp[0] = str[i];
			temp[1] = '\0';
			result = ft_strjoin(result, temp);
			i++;
		}
		else if (str[i] == '$' && !in_single_quote && str[i + 1])
		{
			i++;
			var_name = get_var_name(str, &i);
			if (var_name)
			{
				var_value = expand_var(var_name, env);
				result = ft_strjoin(result, var_value);
				free(var_name);
			}
		}
		else
		{
			temp = malloc(2);
			temp[0] = str[i];
			temp[1] = '\0';
			result = ft_strjoin(result, temp);
			i++;
		}
	}
	return (result);
}

char	*remove_quotes(char *str)
{
	char	*result;
	int		i, j;
	int		in_single_quote;
	int		in_double_quote;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	**expand_args(char **args, t_env *env)
{
	char	**expanded;
	char	*temp;
	char	*unquoted;
	int		i;
	int		count;

	if (!args)
		return (NULL);
	count = 0;
	while (args[count])
		count++;
	expanded = malloc(sizeof(char *) * (count + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (i < count)
	{
		temp = expand_variables(args[i], env);
		unquoted = remove_quotes(temp);
		expanded[i] = unquoted;
		free(temp);
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}