/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:59:38 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/02 20:05:58 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*remove_quotes_advanced(char *str)
{
	char *(result);
	int (quote), i = 0, j = 0;
	if (!str)
		return (NULL);
	result = ft_malloc(ft_strlen(str) + 1, 69);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				result[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else if (str[i] == '\\' && str[i + 1])
		{
			i++;
			result[j++] = str[i++];
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

static int	count_words(char *str)
{
	int	word_count;
	int	in_word;
	int	i;

	word_count = 0;
	in_word = 0;
	i = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			word_count++;
		}
		i++;
	}
	return (word_count);
}

char	**split_unquoted_words(char *str)
{
	char **(words);
	int (word_count), word_idx = 0, i = 0, start;
	if (!str || !*str)
		return (NULL);
	word_count = count_words(str);
	if (word_count == 0)
		return (NULL);
	words = ft_malloc(sizeof(char *) * (word_count + 1), 69);
	while (word_idx < word_count && str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (!str[i])
			break ;
		start = i;
		while (str[i] && !ft_isspace(str[i]))
			i++;
		words[word_idx] = ft_substr(str, start, i - start);
		if (!words[word_idx])
			return (NULL);
		word_idx++;
	}
	words[word_idx] = NULL;
	return (words);
}
