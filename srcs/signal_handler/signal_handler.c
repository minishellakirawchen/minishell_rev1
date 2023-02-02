/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:39 by takira            #+#    #+#             */
/*   Updated: 2023/02/02 23:54:33 by wchen            ###   ########.fr       */
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

void	init_signal_in_prompt(void)
{
	struct sigaction	sig_int_act;

	errno = 0;
	ft_bzero(&sig_int_act, sizeof(sigaction));
	sigemptyset(&sig_int_act.sa_mask);
	sig_int_act.sa_flags = 0;
	sig_int_act.sa_handler = prompt_int_handler;
	if(sigaction(SIGINT, &sig_int_act, NULL) < 0)
		perror("sigaction");
}
