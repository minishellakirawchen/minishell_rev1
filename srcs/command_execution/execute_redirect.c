/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 21:23:30 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 10:22:56 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static int	expand_var_in_heredoc(t_command_info *command_info, t_info *info);
static int connect_redirect_fds(t_command_info **command_info);

int	execute_redirect(t_command_info *command_info, t_info *info)
{

	if (!command_info)
		return (FAILURE);

	if (!command_info->redirect_list)
		return (SUCCESS);

	if (expand_var_in_heredoc(command_info, info) == FAILURE)
		return (FAILURE);

//	dprintf(STDERR_FILENO, "vvv connect redirect_fds vvv\n");
	if (connect_redirect_fds(&command_info) == FAILURE)
		return (FAILURE);
//	dprintf(STDERR_FILENO, "^^^ connect redirect_fds ^^^\n");
	return (SUCCESS);
}

//TODO:is_expand
int	do_expansion_in_heredoc(char *filename, t_info *info)
{
	int			fd;
	t_list_bdi	*save_content;
	t_list_bdi	*list_node;
	char		*line;

	save_content = NULL;
	errno = 0;
	fd = get_fd_and_open_file(filename, e_io_read);
	if (fd < 0)
		return (perror_and_return_int("open", FAILURE));
	while (true)
	{
		line = get_next_line(fd, true);
		if (!line)
			break;
		line = get_expanded_str(line, info);
		list_node = ft_lstnew_bdi(line);
		if (!line || !list_node)
			return (perror_and_return_int("malloc", FAILURE)); //TODO: all free
		ft_lstadd_back_bdi(&save_content, list_node);
	}

	if (close(fd) < 0)
		return (perror_and_return_int("close", FAILURE)); //TODO:all free

	fd = get_fd_and_open_file(filename, e_io_overwrite);
	if (fd < 0)
		return (perror_and_return_int("open", FAILURE));

	list_node = save_content;
	while (list_node)
	{
		line = list_node->content;
		ft_dprintf(fd, line);
		list_node = list_node->next;
	}

	if (close(fd) < 0)
		return (perror_and_return_int("close", FAILURE)); //TODO:all free
	ft_lstclear_bdi(&save_content, free);
	return (SUCCESS);
}

static int	expand_var_in_heredoc(t_command_info *command_info, t_info *info)
{
	t_list_bdi		*redirect_list;
	t_redirect_info	*redirect_info;

	if (!command_info || !info)
		return (FAILURE);

	redirect_list = command_info->redirect_list;
//	debug_print_redirect_info(redirect_list, "expand_var_in_heredoc");

	while (redirect_list)
	{
		redirect_info = redirect_list->content;
		if (redirect_info->io_type == e_heredoc)
		{

			if (do_expansion_in_heredoc(redirect_info->filename, info) == FAILURE)
				return (FAILURE);
		}
		redirect_list = redirect_list->next;
	}
	return (SUCCESS);
}

t_fopen_type	get_fopen_type(t_token_type io_type)
{
	if (io_type == e_redirect_in || io_type == e_heredoc)
		return (e_io_read);
	if (io_type == e_redirect_out)
		return (e_io_overwrite);
	return (e_io_append);
}

int	get_io_fd(t_token_type io_type)
{
	if (io_type == e_redirect_in || io_type == e_heredoc)
		return (FD_INFILE);
	return (FD_OUTFILE);
}

static int connect_redirect_fds(t_command_info **command_info)
{
	t_list_bdi		*redirect_list;
	t_redirect_info	*redirect_info;
	int				fd_idx;
	t_token_type	io_type;

	if (!command_info || !*command_info)
		return (FAILURE);
	if (!(*command_info)->redirect_list)
		return (SUCCESS);
	redirect_list = (*command_info)->redirect_list;

	//open files
	while (redirect_list)
	{
		redirect_info = redirect_list->content;
		io_type = redirect_info->io_type;
		fd_idx = get_io_fd(io_type);
		if ((*command_info)->redirect_fd[fd_idx] != -1)
			if (close((*command_info)->redirect_fd[fd_idx]) < 0)
				return (perror_and_return_int("close", FAILURE));
		(*command_info)->redirect_fd[fd_idx] = get_fd_and_open_file(redirect_info->filename, get_fopen_type(io_type));
		printf("filename:%s, fd:%d\n", redirect_info->filename, (*command_info)->redirect_fd[fd_idx]);
		if ((*command_info)->redirect_fd[fd_idx] < 0)
			return (perror_and_return_int("open", FAILURE));
		redirect_list = redirect_list->next;
	}

	//dup & close
	if ((*command_info)->redirect_fd[FD_INFILE] != -1)
	{
		if (close(STDIN_FILENO) < 0)
			return (perror_and_return_int("close", FAILURE));
		if (dup2((*command_info)->redirect_fd[FD_INFILE], STDIN_FILENO) < 0)
			return (perror_and_return_int("dup2", FAILURE));
		if (close((*command_info)->redirect_fd[FD_INFILE]) < 0)
			return (perror_and_return_int("close", FAILURE));
	}
	if ((*command_info)->redirect_fd[FD_OUTFILE] != -1)
	{
		if (close(STDOUT_FILENO) < 0)
			return (perror_and_return_int("close", FAILURE));
		if (dup2((*command_info)->redirect_fd[FD_OUTFILE], STDOUT_FILENO) < 0)
			return (perror_and_return_int("dup2", FAILURE));
		if (close((*command_info)->redirect_fd[FD_OUTFILE]) < 0)
			return (perror_and_return_int("close", FAILURE));
	}
	return (SUCCESS);
}