/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_signal_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 01:16:00 by wchen             #+#    #+#             */
/*   Updated: 2023/02/06 18:03:02 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gbl_var	g_status;

static void	heredoc_int_handler(int sig_num)
{
	if (sig_num == SIGINT)
		g_status.heredoc_status = EXIT_BY_SIG;
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

int	do_heredoc(int fd, t_redirect_info *r_info)
{
	char	*line;

	if (fd < 0 || !r_info || !r_info->heredoc_eof)
		return (PROCESS_ERROR);
	g_status.heredoc_status = SUCCESS;
	init_signal_heredoc();
	set_tc_attr_out_execute();
	line = NULL;
	while (true)
	{
		ft_dprintf(STDERR_FILENO, "> ");
		line = get_next_line(STDIN_FILENO, true);
		if (g_status.heredoc_status == EXIT_BY_SIG)
			break ;
		if (is_eof(line) || is_delimiter(line, r_info->heredoc_eof))
			break ;
		ft_dprintf(fd, line);
		line = free_1d_alloc(line);
	}
	free_1d_alloc(line);
	if (g_status.heredoc_status)
		ft_dprintf(STDERR_FILENO, "\n");
	return (g_status.heredoc_status);
}
