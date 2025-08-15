/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 20:26:12 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/02 20:55:00 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**expand_arg_array(char **args, t_data *data)
{
	int	total;

	if (!args)
		return (NULL);
	total = count_total_words(args, data);
	if (total == 0)
		return (NULL);
	return (fill_result_array(args, data, total));
}

char	*expand_arg_list(char **args, t_data *data)
{
	char	**expanded;
	char	*joined;

	expanded = expand_arg_array(args, data);
	if (!expanded)
		return (NULL);
	joined = join_strings(expanded);
	return (joined);
}

char	*expand_with_quotes(char *str, t_data *data)
{
	char	*result;

	result = ft_strdup("");
	expand_with_quotes_loop(str, data, &result);
	return (result);
}

char	*expand_dollar(char *str, int *i, t_data *data, int in_single)
{
	int		start;
	char	*name;
	char	*value;

	if (in_single)
		return (add_char(ft_strdup(""), '$'));
	(*i)++;
	if (!str[*i])
		return (ft_strdup("$"));
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(data->status));
	}
	if (!ft_isalpha(str[*i]) && str[*i] != '_')
		return (add_char(ft_strdup(""), '$'));
	start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	name = ft_substr(str, start, *i - start);
	value = ft_strdup(get_env(data, name));
	return (value);
}
