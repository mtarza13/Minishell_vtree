/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_quote.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 20:12:50 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/04 21:15:40 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_quotes(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

static void	split_with_quotes_loop(char *str, char **words, t_split_state *ctx)
{
	while (str[ctx->i])
	{
		if (str[ctx->i] == '\'' && !ctx->state.in_double)
			ctx->state.in_single = !ctx->state.in_single;
		else if (str[ctx->i] == '"' && !ctx->state.in_single)
			ctx->state.in_double = !ctx->state.in_double;
		if (!ctx->state.in_single && !ctx->state.in_double
			&& is_space(str[ctx->i]))
			ctx->state.in_word = 0;
		else if (!ctx->state.in_word)
		{
			ctx->state.in_word = 1;
			extract_and_advance(str, &ctx->i, words, &ctx->word_idx);
			continue ;
		}
		ctx->i++;
	}
}

char	**split_with_quotes(char *str)
{
	char			**words;
	t_split_state	ctx;

	ctx.i = 0;
	ctx.word_idx = 0;
	ctx.state.in_single = 0;
	ctx.state.in_double = 0;
	ctx.state.in_word = 0;
	words = ft_malloc(sizeof(char *) * (count_words_with_quotes(str) + 1), 69);
	split_with_quotes_loop(str, words, &ctx);
	words[ctx.word_idx] = NULL;
	return (words);
}

char	*remove_quotes(char	*str)
{
	char	*result;
	int		i;
	int		j;
	int		in_single;
	int		in_double;

	result = ft_malloc(ft_strlen(str) + 1, 69);
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	expand_with_quotes_loop(char *str, t_data *data, char **result)
{
	int (i), (in_single), (in_double);
	i = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			*result = add_char(*result, str[i++]);
		}
		else if (str[i] == '"' && !in_single)
		{
			in_double = !in_double;
			*result = add_char(*result, str[i++]);
		}
		else if (str[i] == '$' && !in_single)
		{
			*result = ft_strjoin_free(*result, expand_dollar(str, &i, data,
						in_single));
		}
		else
			*result = add_char(*result, str[i++]);
	}
}
