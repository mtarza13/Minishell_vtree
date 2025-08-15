/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_count.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 20:12:48 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/03 23:51:05 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_words_with_quotes(char *str)
{
	int				count;
	int				i;
	t_quote_state	state;

	count = 0;
	i = 0;
	state.in_single = 0;
	state.in_double = 0;
	state.in_word = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !state.in_double)
			state.in_single = !state.in_single;
		else if (str[i] == '"' && !state.in_single)
			state.in_double = !state.in_double;
		else if (!state.in_single && !state.in_double && is_space(str[i]))
			state.in_word = 0;
		else if (!state.in_word)
		{
			state.in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}

int	count_total_words(char **args, t_data *data)
{
	int		total;
	int		i;
	char	**words;
	int		j;

	total = 0;
	i = 0;
	while (args[i])
	{
		if (has_quotes(args[i]))
			total++;
		else
		{
			words = split_with_quotes(expand_with_quotes(args[i], data));
			if (words)
			{
				j = 0;
				while (words[j])
					j++;
				total += j;
			}
		}
		i++;
	}
	return (total);
}

char	*join_strings(char **array)
{
	char	*result;
	size_t	len;
	int		i;
	int		j;

	result = NULL;
	len = 0;
	if (!array)
		return (NULL);
	i = 0;
	while (array[i])
		len += strlen(array[i++]);
	result = ft_malloc(len + i + 1, 1337);
	result[0] = '\0';
	j = 0;
	while (array[j])
	{
		strcat(result, array[j]);
		if (array[j + 1])
			strcat(result, " ");
		j++;
	}
	return (result);
}
