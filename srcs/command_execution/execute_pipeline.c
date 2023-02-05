/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 20:18:47 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 17:44:19 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static int	get_last_status_and_wait_children(t_list_bdi *pipeline_cmds_head)
{
	t_list_bdi		*last_node;
	t_list_bdi		*pipeline_cmds_node;
	t_command_info	*command_info;
	int				exit_status;

	last_node = ft_lstlast_bdi(pipeline_cmds_head);
	pipeline_cmds_node = pipeline_cmds_head;

	while (pipeline_cmds_node)
	{
		command_info = pipeline_cmds_node->content;
		signal(SIGCHLD, SIG_DFL);
		if (waitpid(command_info->pid, &exit_status, 0) < 0)
		{
			signal(SIGCHLD, SIG_IGN);
			return (130);
		}
		exit_status = print_signal_error(exit_status, pipeline_cmds_node, last_node);
		pipeline_cmds_node = pipeline_cmds_node->next;
	}
	return (WEXITSTATUS(exit_status));
}

static int	execute_pipeline_iter(t_list_bdi *pipeline_cmds_head, char **envp, t_info *info)
{
	int				next_pipefd[2];
	int				prev_pipefd[2];
	t_command_info	*command_info;
	t_list_bdi		*pipeline_cmds_node;
	int				exit_status;

	init_pipefd(prev_pipefd, next_pipefd);
	set_tc_attr_in_execute();
	pipeline_cmds_node = pipeline_cmds_head;
	while (pipeline_cmds_node)
	{
		if (pipe(next_pipefd) < 0)
			return (perror_ret_int("pipe", PROCESS_ERROR));
		command_info = pipeline_cmds_node->content;
		command_info->pid = fork();
		if (command_info->pid < 0)
			return (perror_ret_int("fork", PROCESS_ERROR));
		if (is_child_process(command_info->pid))
		{
			init_signal_execute();
			if (dup2_fds( prev_pipefd, next_pipefd,pipeline_cmds_node->next) == PROCESS_ERROR)
				return (PROCESS_ERROR);
			if (close_fds( prev_pipefd, next_pipefd, pipeline_cmds_node->next) == PROCESS_ERROR)
				return (PROCESS_ERROR);
			exit (ft_execve(command_info, envp, info));
		}
		if (is_parent_process(command_info->pid))
		{
			if (close_fds(prev_pipefd, NULL, NULL) == PROCESS_ERROR)
				return (PROCESS_ERROR);
			copy_prevfd_to_newfd(prev_pipefd, next_pipefd);
		}
		pipeline_cmds_node = pipeline_cmds_node->next;
	}
	if (close_fds(next_pipefd, NULL, NULL) == PROCESS_ERROR)
		return (perror_ret_int("close", PROCESS_ERROR));
	exit_status = get_last_status_and_wait_children(pipeline_cmds_head);
	return (exit_status);
}

int	execute_pipeline(t_list_bdi *pipeline_cmds, t_info *info)
{
	int				exit_status;
	t_command_info	*command_info;
	char			**minishell_envp;

	if (!pipeline_cmds|| !pipeline_cmds->content)
		return (PROCESS_ERROR);
	command_info = pipeline_cmds->content;
	if (is_single_builtin(pipeline_cmds))
		return (execute_builtin(info, command_info->commands));
	minishell_envp = create_minishell_envp(info->envlist_head);
	if (!minishell_envp)
		return (PROCESS_ERROR);
	exit_status = execute_pipeline_iter(pipeline_cmds, minishell_envp, info);
	free_2d_alloc((void **)minishell_envp);
	return (exit_status);
}
