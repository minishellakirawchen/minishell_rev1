/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_in_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 10:40:51 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 09:30:05 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/* functions */

static int	expand_var_save_to_list(\
t_list_bdi **savelist, char *filename, t_info *info)
{
	t_list_bdi	*list_node;
	char		*line;
	int			fd;

	errno = 0;
	if (!savelist || !filename)
		return (FAILURE);
	fd = get_openfile_fd(filename, e_io_read);
	if (fd < 0)
		return (perror_and_return_int("open", FAILURE));
	while (true)
	{
		line = get_next_line(fd, true);
		if (!line)
			break ;
		if (expand_var_in_str(&line, info) == FAILURE)
			return (FAILURE);
		list_node = ft_lstnew_bdi(line);
		if (!line || !list_node)
			return (perror_and_return_int("malloc", FAILURE));
		ft_lstadd_back_bdi(savelist, list_node);
	}
	if (close(fd) < 0)
		return (perror_and_return_int("close", FAILURE));
	return (SUCCESS);
}

static int	write_heredoc_file_from_list(t_list_bdi *savelist, char *filename)
{
	int			fd;
	char		*line;
	t_list_bdi	*list_node;

	errno = 0;
	if (!filename)
		return (FAILURE);
	fd = get_openfile_fd(filename, e_io_overwrite);
	if (fd < 0)
		return (perror_and_return_int("open", FAILURE));
	list_node = savelist;
	while (list_node)
	{
		line = list_node->content;
		ft_dprintf(fd, line);
		list_node = list_node->next;
	}
	if (close(fd) < 0)
		return (perror_and_return_int("close", FAILURE));
	return (SUCCESS);
}

int	do_expansion_in_heredoc(char *filename, t_info *info)
{
	t_list_bdi	*save_content;
	int			exit_status;

	save_content = NULL;
	exit_status = SUCCESS;
	if (expand_var_save_to_list(&save_content, filename, info) == FAILURE)
		exit_status = FAILURE;
	if (exit_status == SUCCESS \
	&& write_heredoc_file_from_list(save_content, filename) == FAILURE)
		exit_status = FAILURE;
	ft_lstclear_bdi(&save_content, free);
	return (exit_status);
}
