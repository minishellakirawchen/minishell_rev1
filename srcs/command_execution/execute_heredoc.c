/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 21:49:34 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 18:29:37 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"
#include "minishell.h"

static int	get_filename_and_openfile(\
t_redirect_info **redirect_info, int *cnt, t_command_info **cmd_info)
{
	if (!redirect_info || !cmd_info || !*cmd_info)
		return (PROCESS_ERROR);
	(*redirect_info)->filename = get_heredoc_tmp_filename(*cnt);
	if (!(*redirect_info)->filename)
		return (PROCESS_ERROR);
	(*cmd_info)->redirect_fd[FD_HEREDOC] = get_openfile_fd(
			(*redirect_info)->filename, e_io_overwrite);
	if ((*cmd_info)->redirect_fd[FD_HEREDOC] < 0)
		return (perror_ret_int("open", PROCESS_ERROR));
	return (SUCCESS);
}

static int	closefile_and_judge_status(t_command_info **cmd_info, int exit_val)
{
	if (!cmd_info || !*cmd_info)
		return (PROCESS_ERROR);
	if (close((*cmd_info)->redirect_fd[FD_HEREDOC]) < 0)
		return (perror_ret_int("close", PROCESS_ERROR));
	return (exit_val);
}

static int	do_heredoc_to_tmpfile(t_command_info **cmd_info, int *cnt)
{
	t_list_bdi		*redirect_list;
	t_redirect_info	*r_info;
	int				exit_val;

	if (!cmd_info || !*cmd_info)
		return (FAILURE);
	redirect_list = (*cmd_info)->redirect_list;
	while (redirect_list)
	{
		r_info = redirect_list->content;
		if (r_info->io_type == e_heredoc)
		{
			if (get_filename_and_openfile(\
			&r_info, cnt, cmd_info) == PROCESS_ERROR)
				return (PROCESS_ERROR);
			exit_val = do_heredoc((*cmd_info)->redirect_fd[FD_HEREDOC], r_info);
			if (closefile_and_judge_status(cmd_info, exit_val) != SUCCESS)
				return (exit_val);
			*cnt += 1;
		}
		redirect_list = redirect_list->next;
	}
	return (SUCCESS);
}

static void	move_next_node(t_exec_list **exec_node)
{
	if (!exec_node)
		return ;
	*exec_node = (*exec_node)->next;
	if (*exec_node)
		*exec_node = (*exec_node)->next;
}

int	execute_heredoc(t_exec_list **execlist_head)
{
	t_exec_list		*exec_node;
	t_list_bdi		*command_list_node;
	t_command_info	*cmd_info;
	int				heredoc_cnt;
	int				exit_value;

	if (!execlist_head || !*execlist_head)
		return (PROCESS_ERROR);
	heredoc_cnt = 0;
	exec_node = *execlist_head;
	while (exec_node)
	{
		command_list_node = exec_node->pipeline_commands;
		while (command_list_node)
		{
			cmd_info = command_list_node->content;
			exit_value = do_heredoc_to_tmpfile(&cmd_info, &heredoc_cnt);
			if (exit_value != SUCCESS)
				return (exit_value);
			command_list_node = command_list_node->next;
		}
		move_next_node(&exec_node);
	}
	return (SUCCESS);
}
