/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:14:06 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 00:02:26 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_all(t_mem *mem)
{
	t_mem	*tmp;

	while (mem)
	{
		free(mem->ptr);
		tmp = mem;
		mem = mem->next;
		free(tmp);
	}
}

static void	ft_malloc_failsafe(t_mem *mem)
{
	free_all(mem);
	ft_printf("minishell: malloc somehow failed\n");
	exit(1);
}

void	*ft_malloc(size_t size, int flag)
{
	static t_mem *(head), *(tail);
	t_mem *(new);
	void *(ptr);
	if (!flag)
		return (free_all(head), NULL);
	ptr = (void *)malloc(size);
	if (!ptr)
		ft_malloc_failsafe(head);
	new = (t_mem *)malloc(sizeof(t_mem));
	if (!new)
		(free(ptr), ft_malloc_failsafe(head));
	new->ptr = ptr;
	new->next = NULL;
	if (!head)
	{
		head = new;
		tail = new;
	}
	else
	{
		tail->next = new;
		tail = new;
	}
	return (ptr);
}
