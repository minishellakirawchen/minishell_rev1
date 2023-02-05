/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:39 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 16:59:40 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

 void init_sigaction(int sig_no, struct sigaction sig_act, sigfunc *sig_handler)
{
	errno = 0;

	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = 0;
	sig_act.sa_handler = sig_handler;
	if(sigaction(sig_no, &sig_act, NULL) < 0)
		perror("sigaction");

}

int	print_signal_error(int exit_status, t_list_bdi *node, t_list_bdi *last_node)
{
	if (WIFEXITED(exit_status))
		exit_status = WEXITSTATUS(exit_status);
	else if (WIFSIGNALED(exit_status))
	{
		if (WTERMSIG(exit_status) == 3 && last_node == node)
			ft_printf("Quit: %d\n", WTERMSIG(exit_status));
		exit_status = 131;
	}
	else if (WIFSTOPPED(exit_status))
	{
		ft_printf("stopped by signal: %d\n", WSTOPSIG(exit_status));
		exit_status = 145;
	}
	else if (WIFCONTINUED(exit_status))
	{
		ft_printf("continued\n");
		exit_status = 0;
	}
	return (exit_status);
}
