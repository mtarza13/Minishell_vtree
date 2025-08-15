/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:46:53 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/28 21:50:34 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

static char	*fill_array(const char *s, char c)
{
	char	*word;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (s[len] && s[len] != c)
		len++;
	word = (char *)ft_malloc((len + 1) * sizeof(char), 69);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**array_str;

	i = 0;
	array_str = (char **)ft_malloc(sizeof(char *) * (count_words(s, c) + 1),
			69);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			array_str[i] = fill_array(s, c);
			while (*s && *s != c)
				s++;
			i++;
		}
	}
	array_str[i] = NULL;
	return (array_str);
}
