/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 20:18:47 by takira            #+#    #+#             */
/*   Updated: 2023/01/28 20:21:22 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

int	execute_pipeline(t_list_bdi *pipeline_cmds_head, t_list *envlist_head)
{
	int				exit_status;
	t_command_info	*command_info;
	t_list_bdi		*pipeline_cmds_node;
	t_list_bdi		*last_node;
	int				now_pipefd[2];
	int				prev_pipefd[2];
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

		/* child */
		if (command_info->pid == 0)
		{
			if (dup2_fds(now_pipefd, prev_pipefd, pipeline_cmds_node->next) < 0)
				return (FAILURE);
			if (close_fds(now_pipefd, prev_pipefd, pipeline_cmds_node->next) < 0)
				return (FAILURE);
			exit (ft_execve(command_info->commands, minishell_envp, envlist_head));
		}
		/* parent */
		if (close_fds(NULL, prev_pipefd, NULL) < 0)
			return (FAILURE);
		copy_prevfd_to_newfd(prev_pipefd, now_pipefd);

		pipeline_cmds_node = pipeline_cmds_node->next;
	}

	/* wait */
	last_node = ft_lstlast_bdi(pipeline_cmds_head);
	command_info = last_node->content;
	exit_status = waitpid(command_info->pid, &exit_status, 0);

	pipeline_cmds_node = pipeline_cmds_head;
	while (pipeline_cmds_node)
	{
		command_info = pipeline_cmds_node->content;
		waitpid(command_info->pid, NULL, 0);
		pipeline_cmds_node = pipeline_cmds_node->next;
	}

	minishell_envp = (char **)free_2d_alloc((void **)minishell_envp);
	return (exit_status);
}

