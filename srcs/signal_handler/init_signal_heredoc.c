/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_signal_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 01:16:00 by wchen             #+#    #+#             */
/*   Updated: 2023/02/06 00:19:00 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gbl_var	g_status;

static void	heredoc_int_handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		g_status.heredoc_status = EXIT_FAILURE;
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	init_signal_heredoc(void)
{
	struct sigaction	sig_int_act;
	struct sigaction	sig_quit_act;

	signal(SIGINT, SIG_IGN);
	ft_bzero(&sig_int_act, sizeof(sigaction));
	init_sigaction(SIGINT, sig_int_act, heredoc_int_handler);
	ft_bzero(&sig_quit_act, sizeof(sigaction));
	init_sigaction(SIGQUIT, sig_quit_act, SIG_IGN);
}

int	do_heredoc(int fd, t_redirect_info *redirect_info, int *exit_status)
{
	char	*line;

	g_status.heredoc_status = 0;
	line = NULL;
	if (fd < 0 || !redirect_info || !redirect_info->heredoc_eof)
		return (PROCESS_ERROR);
	init_signal_heredoc();
	set_tc_attr_out_execute();
	while (g_status.heredoc_status == 0)
	{
		ft_dprintf(STDERR_FILENO, "> ");
		line = get_next_line(STDIN_FILENO, true);
		if (is_eof(line) && g_status.heredoc_status == 0)
			return (SUCCESS);
		if (is_delimiter(line, redirect_info->heredoc_eof))
		{
			line = free_1d_alloc(line);
			break ;
		}
		ft_dprintf(fd, line);
		line = free_1d_alloc(line);
	}
	*exit_status = g_status.heredoc_status;
	return (FAILURE);
}
