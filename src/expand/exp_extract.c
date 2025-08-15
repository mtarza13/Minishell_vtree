/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_extract.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 20:12:38 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/02 20:56:30 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	extract_and_advance(const char *str, int *i, char **words,
		int *word_idx)
{
	int				start;
	t_quote_state	state;

	start = *i;
	state.in_single = 0;
	state.in_double = 0;
	while (str[*i] && (state.in_single || state.in_double
			|| !is_space(str[*i])))
	{
		if (str[*i] == '\'' && !state.in_double)
			state.in_single = !state.in_single;
		else if (str[*i] == '"' && !state.in_single)
			state.in_double = !state.in_double;
		(*i)++;
	}
	words[*word_idx] = ft_substr(str, start, *i - start);
	(*word_idx)++;
}

static void	fill_result_line(char **args, t_data *data, char **result, int *idx)
{
	char	**words;
	int		j;

	if (has_quotes(args[0]))
		result[(*idx)++] = remove_quotes(expand_with_quotes(args[0], data));
	else
	{
		words = split_with_quotes(expand_with_quotes(args[0], data));
		if (words)
		{
			j = 0;
			while (words[j])
				result[(*idx)++] = remove_quotes(words[j++]);
		}
	}
}

char	**fill_result_array(char **args, t_data *data, int total)
{
	char	**result;
	int		final_idx;
	int		i;

	final_idx = 0;
	i = 0;
	result = ft_malloc(sizeof(char *) * (total + 1), 1337);
	while (args[i])
	{
		fill_result_line(&args[i], data, result, &final_idx);
		i++;
	}
	result[final_idx] = NULL;
	return (result);
}
