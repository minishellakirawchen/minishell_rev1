/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 21:23:30 by takira            #+#    #+#             */
/*   Updated: 2023/02/04 22:31:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static int	expand_var_in_heredoc(t_command_info *command_info, t_info *info);
static int connect_redirect_fds(t_command_info **command_info);
static int	dup2_and_close_fds(t_command_info **command_info);
static int	open_files(t_command_info **command_info, t_list_bdi *redirect_list);

int	execute_redirect(t_command_info *command_info, t_info *info)
{
	if (!command_info)
		return (FAILURE);
	if (!command_info->redirect_list)
		return (SUCCESS);
	if (expand_var_in_heredoc(command_info, info) == FAILURE)
		return (FAILURE);
	if (connect_redirect_fds(&command_info) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

static int	expand_var_in_heredoc(t_command_info *command_info, t_info *info)
{
	t_list_bdi		*redirect_list;
	t_redirect_info	*redirect_info;

	if (!command_info || !info)
		return (FAILURE);
	redirect_list = command_info->redirect_list;
	while (redirect_list)
	{
		redirect_info = redirect_list->content;
		if (redirect_info->io_type == e_heredoc && redirect_info->is_expansion)
		{
			if (do_expansion_in_heredoc(redirect_info->filename, info) == FAILURE)
				return (FAILURE);
		}
		redirect_list = redirect_list->next;
	}
	return (SUCCESS);
}

static int connect_redirect_fds(t_command_info **command_info)
{
	t_list_bdi		*redirect_list;

	if (!command_info || !*command_info)
		return (FAILURE);
	if (!(*command_info)->redirect_list)
		return (SUCCESS);
	redirect_list = (*command_info)->redirect_list;

	if (open_files(command_info, redirect_list) == FAILURE)
		return (FAILURE);
	if (dup2_and_close_fds(command_info) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

static int	open_files(t_command_info **command_info, t_list_bdi *redirect_list)
{
	t_redirect_info	*redirect_info;
	int				fd_idx;
	t_token_type	io_type;

	if (!command_info || !*command_info || !redirect_list)
		return (FAILURE);
	while (redirect_list)
	{
		redirect_info = redirect_list->content;
		if (redirect_info->is_ambiguous)
		{
			ft_dprintf(STDERR_FILENO, "minishell: %s: ambiguous redirect\n", redirect_info->filename);
			return (FAILURE);
		}
		io_type = redirect_info->io_type;
		fd_idx = get_io_fd(io_type);
		if ((*command_info)->redirect_fd[fd_idx] != -1)
			if (close((*command_info)->redirect_fd[fd_idx]) < 0)
				return (perror_and_return_int("close", FAILURE));
		errno = 0;
		(*command_info)->redirect_fd[fd_idx] = get_openfile_fd(
				redirect_info->filename, get_fopen_type(io_type));
		if ((*command_info)->redirect_fd[fd_idx] < 0)
		{
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", redirect_info->filename,
					   strerror(errno));
			return (FAILURE);
		}
		redirect_list = redirect_list->next;
	}
	return (SUCCESS);
}

static int	dup2_and_close_fds(t_command_info **command_info)
{
	if (!command_info || !*command_info)
		return (FAILURE);
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
