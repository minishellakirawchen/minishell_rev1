/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 21:49:34 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 10:36:34 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static int	create_heredoc_file(t_command_info **command_info, t_redirect_info **redirect_info, int cnt);
static char	*get_heredoc_tmp_filename(int cnt);
static int	do_heredoc(int fd, t_redirect_info *redirect_info);

int	get_fd_and_open_file(const char *filename, t_fopen_type fopen_type)
{
	if (fopen_type == e_read)
		return (open(filename, O_RDONLY));
	if (fopen_type == e_overwrite)
		return (open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644));
	return (open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644));
}

int	execute_heredoc(t_exec_list **pipeline)
{
	t_list_bdi		*command_list_node;
	t_command_info	*command_info;
	t_list_bdi		*redirect_list;
	t_redirect_info	*redirect_info;
	int				heredoc_cnt;

	if (!pipeline)
		return (FAILURE);
	command_list_node = (*pipeline)->pipeline_commands;
	heredoc_cnt = 0;
	while (command_list_node)
	{
		command_info = command_list_node->content;
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
		command_list_node = command_list_node->next;
	}
	return (SUCCESS);
}


// input: hogehoge\n
// delim: hogehoge
bool	is_delimiter(const char *input_line, const char *delimiter)
{
	size_t	input_len;
	size_t	delim_len;

	if (!input_line || !delimiter)
		return (false);
	input_len = ft_strlen_ns(input_line);
	delim_len = ft_strlen_ns(delimiter);
	if ((input_len == delim_len + 1)
	&& ft_strncmp_ns(input_line, delimiter, delim_len) == 0)
		return (true);
	return (false);
}

bool	is_eof(char *line)
{
	return (!line);
}

// is_expand, env

//bash3.2 0 $ cat <<eof
//> $a
//> "$a"
//> eof
//eof
//"eof"

//bash3.2 0 $ cat << $eof
//> hoge
//> $eof
//hoge

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
		line = free_1d_alloc(line);
	}
	return (SUCCESS);
}

static int	create_heredoc_file(t_command_info **cmd_info, t_redirect_info **redirect_info, int cnt, t_list *envlist)
{
	errno = 0;
	if (!cmd_info || !*cmd_info
		|| !redirect_info || !*redirect_info || !(*redirect_info)->heredoc_eof)
		return (FAILURE);

	(*redirect_info)->filename = get_heredoc_tmp_filename(cnt);
	if (!(*redirect_info)->filename)
		return (FAILURE);

	if ((*cmd_info)->redirect_fd[FD_HEREDOC] != -1 && close((*cmd_info)->redirect_fd[FD_HEREDOC]) < 0)
		return (perror_ret_int("close", FAILURE));
	(*cmd_info)->redirect_fd[FD_HEREDOC] = get_fd_and_open_file((*redirect_info)->filename, e_overwrite);
	if ((*cmd_info)->redirect_fd[FD_HEREDOC] < 0)
		return (perror_ret_int("open", FAILURE));
	if (do_heredoc((*cmd_info)->redirect_fd[FD_HEREDOC], *redirect_info, envlist) == FAILURE)
		return (EXIT_FAILURE);
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
