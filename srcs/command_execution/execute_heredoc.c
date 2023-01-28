/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 21:49:34 by takira            #+#    #+#             */
/*   Updated: 2023/01/28 21:54:51 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static int	create_heredoc_file(t_command_info **command_info, t_redirect_info **redirect_info, int cnt);
static char	*get_heredoc_tmp_filename(int cnt);

int execute_heredoc(t_command_info *command_info)
{
	t_list_bdi		*redirect_list;
	t_redirect_info	*redirect_info;
	int				heredoc_cnt;

	if (!command_info)
		return (FAILURE);
	heredoc_cnt = 0;
	redirect_list = command_info->redirect_list;
	while (redirect_list)
	{
		redirect_info = redirect_list->content;
		if (redirect_info->io_type == e_heredoc)
		{
			if (create_heredoc_file(&command_info, &redirect_info, heredoc_cnt) == FAILURE)
				return (FILE_OPEN_ERROR);
			heredoc_cnt++;
		}
		redirect_list = redirect_list->next;
	}
	return (SUCCESS);
}

static int	create_heredoc_file(t_command_info **command_info, t_redirect_info **redirect_info, int cnt)
{
	size_t			idx;
	t_list_bdi		*redirect_list;

	errno = 0;
	if (!command_info || !*command_info
		|| !redirect_info || !*redirect_info || !(*redirect_info)->heredoc_eof)
		return (FAILURE);
	(*redirect_info)->file = get_heredoc_tmp_filename(cnt);
	if (!(*redirect_info)->file)
		return (FAILURE);
	idx = 0;


	while ((*r_info)->heredoc_delims[idx])
	{
		(*r_info)->r_fd[R_FD_HEREDOC] = get_fd_and_open_file((*r_info)->heredoc_file, e_overwrite);
		if ((*r_info)->r_fd[R_FD_HEREDOC] < 0)
			return (perror_and_return_int("open", FAILURE));
		if (execute_heredoc((*r_info)->r_fd[R_FD_HEREDOC], (*r_info)->heredoc_delims[idx]) == FAILURE)
			return (EXIT_FAILURE);
		if (close((*r_info)->r_fd[R_FD_HEREDOC]) < 0)
			return (perror_and_return_int("close", FAILURE));
		idx++;
	}
	if ((*r_info)->input_from == E_HERE_DOC)
	{
		(*r_info)->r_fd[R_FD_HEREDOC] = get_fd_and_open_file((*r_info)->heredoc_file, e_read);
		if ((*r_info)->r_fd[R_FD_HEREDOC] < 0)
			return (perror_and_return_int("open", FAILURE));
		//	ft_printf("file:%s, fd:%d\n", *heredoc_file, *fd);
	}
	return (SUCCESS);
}

// tmpfile: here_doc_<cnt>.tmp
static char	*get_heredoc_tmp_filename(int cnt)
{
	char	*cnt_string;
	char 	*filename;
	size_t	tmp_len;
	size_t	strcnt_len;

	errno = 0;
	cnt_string = ft_itoa(cnt);
	if (!cnt_string)
		return (perror_ret_nullptr("malloc"));
	tmp_len = ft_strlen_ns(HEREDOC_TMP_FILE);
	strcnt_len = ft_strlen_ns(cnt_string);
	filename = (char *)ft_calloc(sizeof(char) ,(tmp_len + strcnt_len + 1));
	if (!filename)
		return (perror_ret_nullptr("malloc"));
	ft_strlcat(filename, HEREDOC_TMP_FILE, tmp_len + strcnt_len + 1);
	ft_strlcat(filename, cnt_string, tmp_len + strcnt_len + 1);
	free(cnt_string);
	return (filename);
}
