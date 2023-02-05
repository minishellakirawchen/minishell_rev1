/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir_for_wildcard.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 12:58:27 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:13:17 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static DIR	*get_dir(void)
{
	DIR				*dir;
	char			*pwd_path;

	errno = 0;
	pwd_path = getcwd(NULL, 0);
	if (!pwd_path)
		return (perror_ret_nullptr("getcwd"));
	dir = opendir(pwd_path);
	free(pwd_path);
	if (!dir)
		return (perror_ret_nullptr("opendir"));
	return (dir);
}

static t_list_bdi	*get_new_node(struct dirent *dirent)
{
	char		*name;
	t_list_bdi	*new_node;

	errno = 0;
	if (!dirent)
		return (NULL);
	name = ft_strdup(dirent->d_name);
	if (!name)
		return (perror_ret_nullptr("malloc"));
	new_node = ft_lstnew_bdi(name);
	if (!new_node)
	{
		free(name);
		return (perror_ret_nullptr("malloc"));
	}
	return (new_node);
}

t_list_bdi	*get_read_dir_list(void)
{
	DIR				*dir;
	struct dirent	*dirent;
	t_list_bdi		*save_list;
	t_list_bdi		*new_node;

	dir = get_dir();
	if (!dir)
		return (NULL);
	save_list = NULL;
	dirent = readdir(dir);
	while (dirent)
	{
		new_node = get_new_node(dirent);
		if (!new_node)
		{
			ft_lstclear_bdi(&save_list, free);
			closedir(dir);
			return (perror_ret_nullptr("malloc"));
		}
		ft_lstadd_back_bdi(&save_list, new_node);
		dirent = readdir(dir);
	}
	closedir(dir);
	return (save_list);
}
