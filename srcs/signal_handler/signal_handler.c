/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:39 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 22:02:25 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_sigaction(int sig_no, struct sigaction sig_act,
		t_sigfunc *sig_handler)
{
	errno = 0;
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = 0;
	sig_act.sa_handler = sig_handler;
	if (sigaction(sig_no, &sig_act, NULL) < 0)
		perror("sigaction");
}

static int	print_int_error(int exit_status, t_list_bdi *node,
		t_list_bdi *last_node)
{
	if (WTERMSIG(exit_status) == SIGQUIT && last_node == node)
	{
		ft_dprintf(STDERR_FILENO, "Quit: %d\n", WTERMSIG(exit_status));
		return (131);
	}
	if (WTERMSIG(exit_status) == SIGINT && last_node == node)
	{
		ft_dprintf(STDERR_FILENO, "\n");
		return (130);
	}
	if (last_node == node)
		ft_dprintf(STDERR_FILENO, "Interrupt by signal : %d\n",
			WTERMSIG(exit_status));
	return (WEXITSTATUS(exit_status));
}

int	print_signal_error(int exit_status, t_list_bdi *node, t_list_bdi *last_node)
{
	if (WIFEXITED(exit_status))
		exit_status = WEXITSTATUS(exit_status);
	else if (WIFSIGNALED(exit_status))
		return (print_int_error(exit_status, node, last_node));
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
