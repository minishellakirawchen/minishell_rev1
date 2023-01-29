/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 20:18:47 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 13:43:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static int	get_last_status_and_wait_children(t_list_bdi *pipeline_cmds_head);
int			execute_pipeline_iter(t_list_bdi **pipeline_cmds_head, char **envp, t_list *envlist);

int	execute_pipeline(t_list_bdi *pipeline_cmds_head, t_list *envlist_head)
{
	int				exit_status;
	t_command_info	*command_info;
	t_list_bdi		*pipeline_cmds_node;
	char			**minishell_envp;

	if (!pipeline_cmds_head)
		return (FAILURE);

	/* vvvvv debug mode: print command_info vvvvv */
	pipeline_cmds_node = pipeline_cmds_head;
	while (pipeline_cmds_node)
	{
		command_info = pipeline_cmds_node->content;
		// tmp print
		debug_print_command_info(command_info);
		pipeline_cmds_node = pipeline_cmds_node->next;
		if (pipeline_cmds_node)
			ft_dprintf(STDERR_FILENO, "       v [pipe:|] v\n");
	}
	/* ^^^^^ debug mode: print command_info ^^^^^ */

	/* execute pipeline commands */
	minishell_envp = create_minishell_envp(envlist_head);
	if (!minishell_envp)
		return (FAILURE);

	if (execute_pipeline_iter(&pipeline_cmds_head, minishell_envp, envlist_head) == FAILURE)
	{
		minishell_envp = (char **)free_2d_alloc((void **)minishell_envp);
		return (FAILURE);
	}

	exit_status = get_last_status_and_wait_children(pipeline_cmds_head);

	minishell_envp = (char **)free_2d_alloc((void **)minishell_envp);
	return (exit_status);
}

int	execute_pipeline_iter(t_list_bdi **pipeline_cmds_head, char **envp, t_list *envlist)
{
	int				now_pipefd[2];
	int				prev_pipefd[2];
	t_command_info	*command_info;
	t_list_bdi		*pipeline_cmds_node;

	if (!pipeline_cmds_head)
		return (FAILURE);
	init_pipefd(prev_pipefd, now_pipefd);
	pipeline_cmds_node = *pipeline_cmds_head;
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
			if (dup2_fds(now_pipefd, prev_pipefd, pipeline_cmds_node->next) < 0)
				return (FAILURE);
			if (close_fds(now_pipefd, prev_pipefd, pipeline_cmds_node->next) < 0)
				return (FAILURE);
			exit (ft_execve(command_info, envp, envlist));
		}
		if (is_parent_process(command_info->pid))
		{
			if (close_fds(NULL, prev_pipefd, NULL) < 0)
				return (FAILURE);
			copy_prevfd_to_newfd(prev_pipefd, now_pipefd);
			pipeline_cmds_node = pipeline_cmds_node->next;
		}
	}
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