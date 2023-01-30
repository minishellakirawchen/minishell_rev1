/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 20:18:47 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 10:15:12 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static int	execute_pipeline_iter(t_list_bdi *pipeline_cmds_head, char **envp, t_info *info);
static int	get_last_status_and_wait_children(t_list_bdi *pipeline_cmds_head);
static bool	is_single_builtin(t_list_bdi *pipeline_cmds_head);

int	execute_pipeline(t_list_bdi *pipeline_cmds_head, t_info *info)
{
	int				exit_status;
	t_command_info	*command_info;
	char			**minishell_envp;
	pid_t			pid;

	if (!pipeline_cmds_head)
		return (FAILURE);
	/* execute pipeline commands */

	command_info = pipeline_cmds_head->content;

	if (is_single_builtin(pipeline_cmds_head))
		return (execute_builtin(info, command_info->commands));

	minishell_envp = create_minishell_envp(info->envlist_head);
	if (!minishell_envp)
		return (FAILURE);

	pid = fork();
	if (pid < 0)
		return (perror_ret_int("fork", FAILURE));
	if (is_child_process(pid))
	{
		if (execute_pipeline_iter(pipeline_cmds_head, minishell_envp, info) == FAILURE)
		{
			free_2d_alloc((void **)minishell_envp);
			exit (FAILURE);
		}
		exit (info->exit_status);
	}
	if (is_parent_process(pid))
	{
		if (waitpid(pid, &exit_status, 0) < 0)
			return (perror_ret_int("waitpid", FAILURE));
		info->exit_status = WEXITSTATUS(exit_status);
	}
	//	exit_status = get_last_status_and_wait_children(pipeline_cmds_head);
	free_2d_alloc((void **)minishell_envp);
	return (info->exit_status);
}

static bool	is_single_builtin(t_list_bdi *pipeline_cmds_head)
{
	t_command_info	*command_info;

	command_info = pipeline_cmds_head->content;
	if (!command_info->commands)
		return (false);
	if (!is_builtin(command_info->commands))
		return (false);
	if (ft_lstsize_bdi(pipeline_cmds_head) > 1)
		return (false);
	if (command_info->redirect_list)
		return (false);
	return (true);
}

static int	execute_pipeline_iter(t_list_bdi *pipeline_cmds_head, char **envp, t_info *info)
{
	int				now_pipefd[2];
	int				prev_pipefd[2];
	t_command_info	*command_info;
	t_list_bdi		*pipeline_cmds_node;
	int				exit_status;

	init_pipefd(prev_pipefd, now_pipefd);

	pipeline_cmds_node = pipeline_cmds_head;
	while (pipeline_cmds_node)
	{
		if (pipe(now_pipefd) < 0)
			return (perror_ret_int("pipe", FAILURE));

		command_info = pipeline_cmds_node->content;
		command_info->pid = fork();
		if (command_info->pid < 0)
			return (perror_ret_int("fork", FAILURE));
		if (is_child_process(command_info->pid))
		{
			if (prev_pipefd[WRITE] != -1)
			{
				if (close(STDIN_FILENO) < 0)
					return (perror_ret_int("close", FAILURE));
				if (dup2(prev_pipefd[READ], STDIN_FILENO) < 0)
					return (perror_ret_int("dup2", FAILURE));
				if (close(prev_pipefd[READ]) < 0 || close(prev_pipefd[WRITE] < 0))
					return (perror_ret_int("close", FAILURE));
			}
			if (pipeline_cmds_node->next)
			{
				if (close(STDOUT_FILENO) < 0)
					return (perror_ret_int("close", FAILURE));
				if (dup2(now_pipefd[WRITE], STDOUT_FILENO) < 0)
					return (perror_ret_int("dup2", FAILURE));
				if (close(now_pipefd[READ]) < 0 || close(now_pipefd[WRITE] < 0))
					return (perror_ret_int("close", FAILURE));
			}

//			if (dup2_fds(now_pipefd, prev_pipefd, pipeline_cmds_node->next) < 0)
//				return (FAILURE);
//			if (close_fds(now_pipefd, prev_pipefd, pipeline_cmds_node->next) < 0)
//				return (FAILURE);
			ft_execve(command_info, envp, info);
			return (CMD_NOT_FOUND);
		}
		if (is_parent_process(command_info->pid))
		{
			if (prev_pipefd[WRITE] != -1)
				if (close(prev_pipefd[READ]) < 0 || close(prev_pipefd[WRITE] < 0))
					return (perror_ret_int("close", FAILURE));

//			if (close_fds(NULL, prev_pipefd, NULL) < 0)
//				return (FAILURE);
			copy_prevfd_to_newfd(prev_pipefd, now_pipefd);
		}
		pipeline_cmds_node = pipeline_cmds_node->next;
	}
	if (close(now_pipefd[READ]) < 0 || close(now_pipefd[WRITE]) < 0)
		return (perror_ret_int("close", FAILURE));
	exit_status = get_last_status_and_wait_children(pipeline_cmds_head);
	info->exit_status = exit_status;
	return (SUCCESS);
}

static int	get_last_status_and_wait_children(t_list_bdi *pipeline_cmds_head)
{
	t_list_bdi		*last_node;
	t_list_bdi		*pipeline_cmds_node;
	t_command_info	*command_info;
	int				exit_status;

	/* wait */
	last_node = ft_lstlast_bdi(pipeline_cmds_head);
	command_info = last_node->content;
	waitpid(command_info->pid, &exit_status, 0);
	pipeline_cmds_node = pipeline_cmds_head;
	while (pipeline_cmds_node)
	{
		command_info = pipeline_cmds_node->content;
		waitpid(command_info->pid, NULL, 0);
		pipeline_cmds_node = pipeline_cmds_node->next;
	}
	return (WEXITSTATUS(exit_status));
}