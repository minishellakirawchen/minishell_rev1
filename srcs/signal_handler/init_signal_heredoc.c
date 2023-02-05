/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_signal_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 01:16:00 by wchen             #+#    #+#             */
/*   Updated: 2023/02/05 22:07:11 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gbl_var g_var;

static void heredoc_int_handler(int sig_num)
{
	if (sig_num == SIGINT)
		g_var.heredoc_status = EXIT_FAILURE;
}

void	init_signal_heredoc(void)
{
	struct sigaction	sig_int_act;
	struct sigaction	sig_quit_act;

	ft_bzero(&sig_int_act, sizeof(sigaction));
	ft_bzero(&sig_quit_act, sizeof(sigaction));
	init_sigaction(SIGINT, sig_int_act, heredoc_int_handler);
	init_sigaction(SIGQUIT, sig_quit_act, SIG_IGN);
}

int	do_heredoc(int fd, t_redirect_info *redirect_info)
{
	char	*line;

	if (fd < 0 || !redirect_info || !redirect_info->heredoc_eof)
		return (FAILURE);
	init_signal_heredoc();
	while (g_var.heredoc_status == 0)
	{
		ft_dprintf(STDERR_FILENO, "> ");
		line = get_next_line(STDIN_FILENO, true);
		if (is_eof(line))
			return (SUCCESS);
		if (is_delimiter(line, redirect_info->heredoc_eof))
		{
			line = free_1d_alloc(line);
			break ;
		}
		ft_dprintf(fd, line);
		line = free_1d_alloc(line);
	}
	if (g_var.heredoc_status == EXIT_FAILURE && line != NULL)
		line = free_1d_alloc(line);
	return (g_var.heredoc_status);
}
