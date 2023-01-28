/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 19:34:52 by takira            #+#    #+#             */
/*   Updated: 2023/01/28 19:54:50 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static int	ft_execvp(char **commands, char **minishell_envp, t_list *envlist);
static bool	is_path(const char *commands_head);
static char	*get_execute_path(char *path, char *file);

int	ft_execve(char **commands, char **minishell_envp, t_list *envlist)
{
	if (!commands || !*commands || !minishell_envp || !envlist)
		return (FAILURE);

	// connect_redirect_fd()

	// is_builtin

	// execute commands

	if (is_path(commands[0]))
		execve(commands[0], commands, minishell_envp);
	else
		ft_execvp(commands, minishell_envp, envlist);
	ft_dprintf(STDERR_FILENO, "command not found: %s\n", commands[0]);
	return (CMD_NOT_FOUND);
}

static int	ft_execvp(char **commands, char **minishell_envp, t_list *envlist)
{
	char		**splitted_paths;
	char		*env_paths;
	size_t		idx;
	char 		*path;

	if (!commands)
		return (FAILURE);
	errno = 0;
	env_paths = get_env_value(PATH, envlist);
	splitted_paths = ft_split(env_paths, CHA_PATH_DELIM);
	if (!splitted_paths)
		return (perror_ret_int("malloc", FAILURE));
	idx = 0;
	while (splitted_paths[idx])
	{
		path = get_execute_path(splitted_paths[idx], commands[0]);
		if (!path)
			return (perror_ret_int("malloc", FAILURE));
//		dprintf(STDERR_FILENO, "debug: cmdpath:%s\n", path);
		execve(path, commands, minishell_envp);
		path = free_1d_alloc(path);
		idx++;
	}
	splitted_paths = (char **)free_2d_alloc((void **)splitted_paths);
	return (CMD_NOT_FOUND);
}

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
	len++;
	execute_path = (char *)ft_calloc(sizeof(char), len);
	if (!execute_path)
		return (perror_ret_nullptr("malloc"));
	ft_strlcat(execute_path, path, len);
	if (path_len > 0 && path[path_len - 1] != '/')
		ft_strlcat(execute_path, "/", len);
	ft_strlcat(execute_path, file, len);
//	printf("create path:[%s]\n", execute_path); // check
	return (execute_path);
}

static bool	is_path(const char *commands_head)
{
	if (!commands_head)
		return (false);
	if (commands_head[0] == '/' || commands_head[0] == '.')
		return (true);
	return (false);
}