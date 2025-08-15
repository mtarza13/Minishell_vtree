/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 14:39:39 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 00:10:30 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*append_character(char *string, char c)
{
	char	*str;
	int		len;
	int		i;

	if (!string)
	{
		string = ft_strdup(&c);
		return (string);
	}
	len = ft_strlen(string);
	str = ft_malloc(len + 2, 1337);
	i = -1;
	while (string[++i])
		str[i] = string[i];
	str[i] = c;
	str[i + 1] = string[i];
	return (str);
}

static void	ft_putchar_ft(char c, int flag)
{
	static char	*string = NULL;

	string = append_character(string, c);
	if (!flag)
	{
		write(2, string, ft_strlen(string));
		string = NULL;
	}
}

static void	ft_putstr_ft(char *s)
{
	int	i;

	i = -1;
	if (s == NULL)
		return (ft_putstr_ft("(null)"));
	while (s[++i])
		ft_putchar_ft(s[i], 1337);
}

static void	check(va_list args, const char *s)
{
	int		i;
	char	t;

	i = 0;
	while (s[i])
	{
		if (s[i] == '%')
		{
			t = s[i + 1];
			if (t == 's')
				ft_putstr_ft(va_arg(args, char *));
			i++;
		}
		else if (!s[i + 1])
			ft_putchar_ft(s[i], 0);
		else
			ft_putchar_ft(s[i], 1337);
		i++;
	}
}

void	ft_printf(const char *s, ...)
{
	va_list	args;

	if (!s)
		return ;
	va_start(args, s);
	check(args, s);
	va_end(args);
}
