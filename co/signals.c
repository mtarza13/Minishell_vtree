/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-23 10:19:53 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-23 10:19:53 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal_received = 0;

void handle_signal(int signo) {
    if (signo == SIGINT) {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_signal_received = CTRL_C_EXIT;
    }
}

void setup_signals(void) {
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, SIG_IGN);
}