/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:29:08 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/04 03:24:28 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_signal(int signo)
{
	(void)signo;
	g_signal_received = 1;
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_heredoc_signal(int signo)
{
	(void)signo;
	ft_malloc(0, 0);
	exit(130);
}

void	handle_sigint_heredoc(int signo)
{
	(void)signo;
	g_signal_received = 1;
}

void	signals_execute_child(int signo)
{
	ft_malloc(0, 0);
	exit(signo + 128);
}
