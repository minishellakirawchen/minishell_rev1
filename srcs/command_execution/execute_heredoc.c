/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 21:49:34 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 00:21:08 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"
#include "minishell.h"

static int	get_filename_and_openfile(\
t_redirect_info **redirect_info, int *cnt, t_command_info **cmd_info)
{
	if (!redirect_info || !cmd_info || !*cmd_info)
		return (FAILURE);
	(*redirect_info)->filename = get_heredoc_tmp_filename(*cnt);
	if (!(*redirect_info)->filename)
		return (FAILURE);
	(*cmd_info)->redirect_fd[FD_HEREDOC] = get_openfile_fd(
			(*redirect_info)->filename, e_io_overwrite);
	if ((*cmd_info)->redirect_fd[FD_HEREDOC] < 0)
		return (perror_ret_int("open", FAILURE));
	return (SUCCESS);
}

static int	closefile_and_judge_status(t_command_info **cmd_info, int status)
{
	if (!cmd_info || !*cmd_info)
		return (FAILURE);
	if (close((*cmd_info)->redirect_fd[FD_HEREDOC]) < 0)
		return (perror_ret_int("close", FAILURE));
	if (status == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

static int	create_heredoc_file_in_cmd_info(t_command_info **cmd_info,
	int *cnt, int *exit_status)
{
	t_list_bdi		*redirect_list;
	t_redirect_info	*r_info;

	if (!cmd_info || !*cmd_info)
		return (FAILURE);
	redirect_list = (*cmd_info)->redirect_list;
	while (redirect_list)
	{
		r_info = redirect_list->content;
		if (r_info->io_type == e_heredoc)
		{
			if (get_filename_and_openfile(&r_info, cnt, cmd_info) == FAILURE)
				return (FAILURE);
			if (do_heredoc(\
				(*cmd_info)->redirect_fd[FD_HEREDOC],
					r_info, exit_status) == PROCESS_ERROR)
				return (PROCESS_ERROR);
			if (closefile_and_judge_status(cmd_info, *exit_status) == FAILURE)
				return (FAILURE);
			*cnt += 1;
		}
		redirect_list = redirect_list->next;
	}
	return (SUCCESS);
}

int	execute_heredoc(t_exec_list **execlist_head, int *exit_status)
{
	t_exec_list		*exec_node;
	t_list_bdi		*command_list_node;
	t_command_info	*command_info;
	int				heredoc_cnt;

	if (!execlist_head || !*execlist_head)
		return (PROCESS_ERROR);
	heredoc_cnt = 0;
	exec_node = *execlist_head;
	while (exec_node)
	{
		command_list_node = exec_node->pipeline_commands;
		while (command_list_node)
		{
			command_info = command_list_node->content;
			if (create_heredoc_file_in_cmd_info(\
			&command_info, &heredoc_cnt, exit_status) == PROCESS_ERROR)
				return (PROCESS_ERROR);
			command_list_node = command_list_node->next;
		}
		exec_node = exec_node->next;
		if (exec_node)
			exec_node = exec_node->next;
	}
	return (SUCCESS);
}
