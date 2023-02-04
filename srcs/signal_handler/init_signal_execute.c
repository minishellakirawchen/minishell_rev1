/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_signal_execute.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 01:16:00 by wchen             #+#    #+#             */
/*   Updated: 2023/02/05 01:28:15 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void execute_int_handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		kill(0, SIGKILL);
		exit (130);
	}
}

static void execute_quit_handler(int sig_num)
{
	if (sig_num == SIGQUIT)
		exit (131);
}

void	init_signal_execute(void)
{
	struct sigaction	sig_int_act;
	struct sigaction	sig_quit_act;

	ft_bzero(&sig_int_act, sizeof(sigaction));
	ft_bzero(&sig_quit_act, sizeof(sigaction));
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	init_sigaction(SIGINT, sig_int_act, execute_int_handler);
	init_sigaction(SIGQUIT, sig_quit_act, execute_quit_handler);
}
