/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 20:18:47 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 20:56:21 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static int	get_last_status_and_wait_children(t_list_bdi *pipeline_cmds_head)
{
	t_list_bdi		*last_node;
	t_list_bdi		*cmds_node;
	t_command_info	*command_info;
	int				exit_status;

	last_node = ft_lstlast_bdi(pipeline_cmds_head);
	cmds_node = pipeline_cmds_head;
	while (cmds_node)
	{
		command_info = cmds_node->content;
		signal(SIGCHLD, SIG_DFL);
		if (waitpid(command_info->pid, &exit_status, 0) < 0)
		{
			signal(SIGCHLD, SIG_IGN);
			return (130);
		}
		exit_status = print_signal_error(exit_status, cmds_node, last_node);
		cmds_node = cmds_node->next;
	}
	return (exit_status);
}

static int	exec_pipe_and_fork(int next_pipefd[2], \
t_command_info **cmd_info, t_list_bdi *cmd_node)
{
	if (!cmd_info || !cmd_info)
		return (PROCESS_ERROR);
	if (pipe(next_pipefd) < 0)
		return (perror_ret_int("pipe", PROCESS_ERROR));
	(*cmd_info) = cmd_node->content;
	(*cmd_info)->pid = fork();
	if ((*cmd_info)->pid < 0)
		return (perror_ret_int("fork", PROCESS_ERROR));
	return (SUCCESS);
}

static int	handle_fd_and_signal4child(\
int prev_pipefd[2], int next_pipefd[2], t_list_bdi *node)
{
	if (!node)
		return (FAILURE);
	init_signal_execute();
	if (dup2_fds(prev_pipefd, next_pipefd, node->next) == PROCESS_ERROR)
		return (FAILURE);
	if (close_fds(prev_pipefd, next_pipefd, node->next) == PROCESS_ERROR)
		return (FAILURE);
	return (SUCCESS);
}

static int	handle_fd_for_parent(int prev_pipefd[2], int next_pipefd[2])
{
	if (close_fds(prev_pipefd, NULL, NULL) == PROCESS_ERROR)
		return (PROCESS_ERROR);
	copy_prevfd_to_newfd(prev_pipefd, next_pipefd);
	return (SUCCESS);
}

int	execute_pipe_iter(t_list_bdi *pipeline, char **envp, t_info *info)
{
	int				next_pipefd[2];
	int				prev_pipefd[2];
	t_list_bdi		*cmd_node;
	t_command_info	*cmd_info;

	init_pipefd(prev_pipefd, next_pipefd);
	set_tc_attr_in_execute();
	cmd_node = pipeline;
	while (cmd_node)
	{
		if (exec_pipe_and_fork(next_pipefd, &cmd_info, cmd_node) == FAILURE)
			return (PROCESS_ERROR);
		if (is_child_process(cmd_info->pid))
		{
			if (!handle_fd_and_signal4child(prev_pipefd, next_pipefd, cmd_node))
				exit (PROCESS_ERROR);
			exit (ft_execve(cmd_info, envp, info));
		}
		if (handle_fd_for_parent(prev_pipefd, next_pipefd) == PROCESS_ERROR)
			return (PROCESS_ERROR);
		cmd_node = cmd_node->next;
	}
	if (close_fds(next_pipefd, NULL, NULL) == PROCESS_ERROR)
		return (perror_ret_int("close", PROCESS_ERROR));
	return (get_last_status_and_wait_children(pipeline));
}
