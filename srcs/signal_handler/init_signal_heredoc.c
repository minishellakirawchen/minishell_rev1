/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_signal_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 01:16:00 by wchen             #+#    #+#             */
/*   Updated: 2023/02/09 13:53:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>

t_gbl_var	g_status;

static void	heredoc_int_handler(int sig_num)
{
	if (sig_num == SIGINT)
		g_status.heredoc_status = EXIT_BY_SIG;
}

static void	init_signal_heredoc(void)
{
	struct sigaction	sig_int_act;
	struct sigaction	sig_quit_act;

	ft_bzero(&sig_int_act, sizeof(sigaction));
	init_sigaction(SIGINT, sig_int_act, heredoc_int_handler);
	ft_bzero(&sig_quit_act, sizeof(sigaction));
	init_sigaction(SIGQUIT, sig_quit_act, SIG_IGN);
}

static int	heredoc_signal_event(void)
{
	if (g_status.heredoc_status == EXIT_BY_SIG)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		rl_done = true;
	}
	return (0);
}

static void	init_signal_in_heredoc(void)
{
	g_status.heredoc_status = SUCCESS;
	init_signal_heredoc();
	set_tc_attr_out_execute();
	rl_event_hook = heredoc_signal_event;
}

int	do_heredoc(int fd, t_redirect_info *r_info)
{
	char	*line;
	int		exit_val;

	if (fd < 0 || !r_info || !r_info->heredoc_eof)
		return (PROCESS_ERROR);
	init_signal_in_heredoc();
	line = NULL;
	while (true)
	{
		line = readline("> ");
		if (is_same_str(line, "") && g_status.heredoc_status == EXIT_BY_SIG)
			break ;
		if (is_eof(line) || is_same_str(line, r_info->heredoc_eof))
			break ;
		ft_dprintf(fd, line);
		ft_dprintf(fd, "\n");
		line = free_ret_nullprt(line);
	}
	free_ret_nullprt(line);
	exit_val = g_status.heredoc_status;
	g_status.heredoc_status = SUCCESS;
	return (exit_val);
}
