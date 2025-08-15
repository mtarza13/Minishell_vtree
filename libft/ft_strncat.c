/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:55:11 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 03:32:59 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strncat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	if ((!src && size == 0) || (!dest && size == 0))
		return ;
	i = 0;
	j = ft_strlen(dest);
	if (size == 0 || j > size)
		return ;
	while (j + i < size - 1 && src[i])
	{
		dest[j + i] = src[i];
		i++;
	}
	dest[j + i] = '\0';
}
