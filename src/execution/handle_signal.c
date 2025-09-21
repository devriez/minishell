/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:17:27 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/19 20:33:15 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_last_signal = SIGINT;
}

void	ignore_sigint_print_newline(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	g_last_signal = SIGINT;
}

void	signal_handler_wait(int signum)
{
	(void)signum;
}

void	setup_signals_to_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}



void	setup_signals(void (*signal_handler)(int))
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}
