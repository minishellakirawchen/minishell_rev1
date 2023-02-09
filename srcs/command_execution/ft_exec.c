/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 19:34:52 by takira            #+#    #+#             */
/*   Updated: 2023/02/09 11:41:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static char	*get_execute_path(char *path, char *file)
{
	char			*execute_path;
	size_t			len;
	const size_t	path_len = ft_strlen_ns(path);
	const size_t	file_len = ft_strlen_ns(file);

	errno = 0;
	len = path_len + file_len;
	if (path_len > 0 && path[path_len - 1] != '/')
		len++;
	execute_path = (char *)ft_calloc(sizeof(char), len + 1);
	if (!execute_path)
		return (perror_ret_nullptr("malloc"));
	ft_strlcat_ns(execute_path, path, len + 1);
	if (path_len > 0 && path[path_len - 1] != '/')
		ft_strlcat_ns(execute_path, "/", len + 1);
	ft_strlcat_ns(execute_path, file, len + 1);
	return (execute_path);
}

static int	do_execve(char **splitted_paths, char **envp, char **commands)
{
	size_t		idx;
	char		*path;

	if (!splitted_paths || !commands)
		return (PROCESS_ERROR);
	idx = 0;
	while (splitted_paths[idx])
	{
		if (ft_strlen_ns(splitted_paths[idx]) == 0)
		{
			splitted_paths[idx] = free_ret_nullprt(splitted_paths[idx]);
			splitted_paths[idx] = ft_strdup(PATH_CURRENT);
			if (!splitted_paths[idx])
				return (PROCESS_ERROR);
		}
		path = get_execute_path(splitted_paths[idx], commands[0]);
		if (!path)
			return (PROCESS_ERROR);
		execve(path, commands, envp);
		free_ret_nullprt(path);
		idx++;
	}
	return (CMD_NOT_FOUND);
}

static int	ft_execvp(char **commands, char **minishell_envp, t_list *envlist)
{
	char	**splitted_paths;
	char	*env_paths;
	int		exit_val;

	if (!commands)
		return (PROCESS_ERROR);
	errno = 0;
	env_paths = get_env_value(PATH, envlist);
	splitted_paths = ft_split_empty(env_paths, CHA_PATH_DELIM);
	if (!splitted_paths)
		return (perror_ret_int("malloc", PROCESS_ERROR));
	exit_val = do_execve(splitted_paths, minishell_envp, commands);
	free_2d_alloc((void **)splitted_paths);
	ft_dprintf(STDERR_FILENO, ERRMSG_CMD_NOT_FOUND, commands[0]);
	return (exit_val);
}

static bool	is_path(const char *commands_head)
{
	if (commands_head \
	&& (commands_head[0] == '/' \
	|| (commands_head[0] == '.' && commands_head[1] == '/')))
		return (true);
	return (false);
}

static void	check_is_a_dir_exit_if_dir(const char *commands_head)
{
	const size_t	len = ft_strlen_ns(commands_head);
	size_t			slash_cnt;
	size_t			dot_cnt;

	slash_cnt = cnt_chr_in_str('/', commands_head);
	dot_cnt = cnt_chr_in_str('.', commands_head);
	if (len == 1 && dot_cnt == 1)
	{
		ft_dprintf(STDERR_FILENO, \
		"minishell: %s: filename argument required\n", commands_head);
		exit (FILENAME_REQUIRED);
	}
	if (len == slash_cnt + dot_cnt)
	{
		ft_dprintf(STDERR_FILENO, \
		"minishell: %s: is a directory\n", commands_head);
		exit (IS_A_DIR);
	}
}

static	int	exec_execve(t_command_info *cmd_info, char **minishell_envp)
{
	int	access_ret;
	int	save_errno;

	errno = 0;
	access_ret = access(cmd_info->commands[0], X_OK);
	if (access_ret < 0)
	{
		save_errno = errno;
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd_info->commands[0],
				   strerror(save_errno));
		if (save_errno == EACCES)
			return (PERMISSION_DENIED);
		return (CMD_NOT_FOUND);
	}
	execve(cmd_info->commands[0], cmd_info->commands, minishell_envp);
	return (CMD_NOT_FOUND);
}

int	ft_execve(t_command_info *cmd_info, char **minishell_envp, t_info *info)
{
	if (!cmd_info || !minishell_envp || !info)
		exit (PROCESS_ERROR);
	if (execute_redirect(cmd_info, info) == FAILURE)
		exit (FILE_OPEN_ERROR);
	if (!cmd_info->subshell_token_list && get_arr_size(cmd_info->commands) == 0)
		exit (EXIT_SUCCESS);
	if (is_builtin(cmd_info->commands))
		exit (execute_builtin(info, cmd_info->commands, true));
	if (cmd_info->subshell_token_list)
		exit (execute_subshell(&cmd_info->subshell_token_list, info));
	check_is_a_dir_exit_if_dir(cmd_info->commands[0]);
	if (is_path(cmd_info->commands[0]))
		exit (exec_execve(cmd_info, minishell_envp));
	exit (ft_execvp(cmd_info->commands, minishell_envp, \
	info->envlist_head));
}

