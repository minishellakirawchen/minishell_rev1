/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:39 by takira            #+#    #+#             */
/*   Updated: 2023/02/03 00:59:19 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void prompt_int_handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		// rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void init_sigaction(int sig_no, struct sigaction sig_act, sigfunc *sig_handler)
{
	errno = 0;

	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = 0;
	sig_act.sa_handler = sig_handler;
	if(sigaction(sig_no, &sig_act, NULL) < 0)
		perror("sigaction");

}

void	init_signal_in_prompt(void)
{
	struct sigaction	sig_int_act;
	struct sigaction	sig_quit_act;

	ft_bzero(&sig_int_act, sizeof(sigaction));
	init_sigaction(SIGINT, sig_int_act, prompt_int_handler);
	ft_bzero(&sig_quit_act, sizeof(sigaction));
	init_sigaction(SIGQUIT, sig_quit_act, SIG_IGN);
}
