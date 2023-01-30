/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 20:16:04 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 10:31:36 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

void	init_pipefd(int prev_pipefd[2], int now_pipefd[2])
{
	if (!prev_pipefd || !now_pipefd)
		return ;
	prev_pipefd[READ] = STDIN_FILENO;
	prev_pipefd[WRITE] = -1;
	now_pipefd[READ] = -1;
	now_pipefd[WRITE] = -1;
}

void	copy_prevfd_to_newfd(int prev_pipefd[2], const int now_pipefd[2])
{
	if (!prev_pipefd || !now_pipefd)
		return ;
	prev_pipefd[READ] = now_pipefd[READ];
	prev_pipefd[WRITE] = now_pipefd[WRITE];
}

int	dup2_fds(int now_fd[2], int prev_fd[2], t_list_bdi *next)
{
	errno = 0;
	if (prev_fd && prev_fd[WRITE] != -1)
	{
		if (close(STDIN_FILENO) < 0)
			return (perror_ret_int("close", FAILURE));
		if (dup2(prev_fd[READ], STDIN_FILENO) < 0)
			return (perror_ret_int("dup2", FAILURE));
	}
	if (next && now_fd)
	{
		if (close(STDOUT_FILENO) < 0)
			return (perror_ret_int("close", FAILURE));
		if (dup2(now_fd[WRITE], STDOUT_FILENO) < 0)
			return (perror_ret_int("dup2", FAILURE));
	}
	return (SUCCESS);
}

int	close_fds(int now_fd[2], int prev_fd[2], t_list_bdi *next)
{
	errno = 0;
	if (prev_fd && prev_fd[WRITE] != -1)
		if (close(prev_fd[READ]) < 0 || close(prev_fd[WRITE]) < 0)
			return (perror_ret_int("close", FAILURE));
	if (next && now_fd)
		if (close(now_fd[READ]) < 0 || close(now_fd[WRITE]) < 0)
			return (perror_ret_int("close", FAILURE));
	return (SUCCESS);
}

