/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 21:49:34 by takira            #+#    #+#             */
/*   Updated: 2023/02/01 23:55:01 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"
//static int	create_heredoc_file(t_command_info **cmd_info, t_redirect_info **redirect_info, int cnt);
static int	do_heredoc(int fd, t_redirect_info *redirect_info);
static int	create_heredoc_file_in_command_info(t_command_info **cmd_info, int *heredoc_cnt);

int	execute_heredoc(t_exec_list **execlist_head)
{
	t_exec_list		*exec_node;
	t_list_bdi		*command_list_node;
	t_command_info	*command_info;
	int				heredoc_cnt;

	if (!execlist_head || !*execlist_head)
		return (FAILURE);
	heredoc_cnt = 0;
	exec_node = *execlist_head;
	while (exec_node)
	{
		command_list_node = exec_node->pipeline_commands;
		while (command_list_node)
		{
			command_info = command_list_node->content;
			create_heredoc_file_in_command_info(&command_info, &heredoc_cnt);
			command_list_node = command_list_node->next;
		}
		exec_node = exec_node->next;
		if (exec_node)
			exec_node = exec_node->next;//skip operator node
	}
	return (SUCCESS);
}

static int	create_heredoc_file_in_command_info(t_command_info **cmd_info, int *heredoc_cnt)
{
	t_list_bdi		*redirect_list;
	t_redirect_info	*redirct_info;

	if (!cmd_info || !*cmd_info)
		return (FAILURE);
	errno = 0;
	redirect_list = (*cmd_info)->redirect_list;
	while (redirect_list)
	{
		redirct_info = redirect_list->content;
		if (redirct_info->io_type == e_heredoc)
		{
			redirct_info->filename = get_heredoc_tmp_filename(*heredoc_cnt);
			if (!redirct_info->filename)
				return (FAILURE);
			(*cmd_info)->redirect_fd[FD_HEREDOC] = get_openfile_fd(redirct_info->filename, e_io_overwrite);
			if ((*cmd_info)->redirect_fd[FD_HEREDOC] < 0)
				return (perror_ret_int("open", FAILURE));
			if (do_heredoc((*cmd_info)->redirect_fd[FD_HEREDOC], redirct_info) == FAILURE)
				return (FAILURE);
			if (close((*cmd_info)->redirect_fd[FD_HEREDOC]) < 0)
				return (perror_ret_int("close", FAILURE));
			*heredoc_cnt += 1;
		}
		redirect_list = redirect_list->next;
	}
	return (SUCCESS);
}

/*
static int	create_heredoc_file(t_command_info **cmd_info, t_redirect_info **redirect_info, int cnt)
{
	errno = 0;
	if (!cmd_info || !*cmd_info
		|| !redirect_info || !*redirect_info || !(*redirect_info)->heredoc_eof)
		return (FAILURE);

	(*redirect_info)->filename = get_heredoc_tmp_filename(cnt);
	if (!(*redirect_info)->filename)
		return (FAILURE);
	(*cmd_info)->redirect_fd[FD_HEREDOC] = get_openfile_fd((*redirect_info)->filename, e_io_overwrite);
	if ((*cmd_info)->redirect_fd[FD_HEREDOC] < 0)
		return (perror_ret_int("open", FAILURE));
	if (do_heredoc((*cmd_info)->redirect_fd[FD_HEREDOC], *redirect_info) == FAILURE)
		return (EXIT_FAILURE);
	if (close((*cmd_info)->redirect_fd[FD_HEREDOC]) < 0)
		return (perror_ret_int("close", FAILURE));
	return (SUCCESS);
}
*/
//bash3.2 0 $ cat <<eof ;;where $a=eof
//> $a		//eof
//> "$a"	//"eof"
//> eof

//bash3.2 0 $ cat << $eof ;;where $eof=hoge
//> hoge	//hoge
//> $eof

/* expand in command execution */
static int	do_heredoc(int fd, t_redirect_info *redirect_info)
{
	char	*line;

	if (fd < 0 || !redirect_info || !redirect_info->heredoc_eof)
		return (FAILURE);
	while (true)
	{
		ft_dprintf(STDERR_FILENO, "> ");
		line = get_next_line(STDIN_FILENO, true);
		if (is_eof(line))
			return (SUCCESS);
		if (is_delimiter(line, redirect_info->heredoc_eof))
		{
			free_1d_alloc(line);
			break ;
		}
		ft_dprintf(fd, line);
		free_1d_alloc(line);
	}
	return (SUCCESS);
}

