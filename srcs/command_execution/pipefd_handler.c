/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipefd_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 20:16:04 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 22:06:05 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

void	init_pipefd_term(int prev_pipefd[2], int next_pipefd[2])
{
	set_tc_attr_in_execute();
	if (!prev_pipefd || !next_pipefd)
		return ;
	prev_pipefd[READ] = STDIN_FILENO;
	prev_pipefd[WRITE] = -1;
	next_pipefd[READ] = -1;
	next_pipefd[WRITE] = -1;
}

void	copy_prevfd_to_newfd(int prev_pipefd[2], const int next_pipefd[2])
{
	if (!prev_pipefd || !next_pipefd)
		return ;
	prev_pipefd[READ] = next_pipefd[READ];
	prev_pipefd[WRITE] = next_pipefd[WRITE];
}

int	dup2_fds(int prev_pipefd[2], int next_pipefd[2], t_list_bdi *next)
{
	errno = 0;
	if (prev_pipefd && prev_pipefd[WRITE] != -1)
	{
		if (close(STDIN_FILENO) < 0)
			return (perror_ret_int("close", PROCESS_ERROR));
		if (dup2(prev_pipefd[READ], STDIN_FILENO) < 0)
			return (perror_ret_int("dup2", PROCESS_ERROR));
	}
	if (next && next_pipefd)
	{
		if (close(STDOUT_FILENO) < 0)
			return (perror_ret_int("close", PROCESS_ERROR));
		if (dup2(next_pipefd[WRITE], STDOUT_FILENO) < 0)
			return (perror_ret_int("dup2", PROCESS_ERROR));
	}
	return (SUCCESS);
}

int	close_fds(int prev_pipefd[2], int next_pipefd[2], t_list_bdi *next)
{
	errno = 0;
	if (prev_pipefd && prev_pipefd[WRITE] != -1)
		if (close(prev_pipefd[READ]) < 0 || close(prev_pipefd[WRITE]) < 0)
			return (perror_ret_int("close", PROCESS_ERROR));
	if (next && next_pipefd)
		if (close(next_pipefd[READ]) < 0 || close(next_pipefd[WRITE]) < 0)
			return (perror_ret_int("close", PROCESS_ERROR));
	return (SUCCESS);
}
