/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir_setpath.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 12:39:43 by wchen             #+#    #+#             */
/*   Updated: 2023/02/05 18:58:23 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_append_path(t_info *info, t_cd_info *cd_info, char *env_pwd)
{
	int	exit_status;

	exit_status = 0;
	exit_status += append_env(info, ft_strdup("PWD+"), ft_strjoin("/",
				cd_info->newpwd));
	exit_status += add_env(info, ft_strdup("OLDPWD"), ft_strdup_ns(env_pwd));
	return (exit_status);
}

static int	set_path(t_info *info, t_cd_info *cd_info)
{
	int		exit_status;
	char	*env_pwd;

	if (!cd_info->env_pwd)
		env_pwd = NULL;
	else
		env_pwd = *cd_info->env_pwd;
	exit_status = 0;
	if (*cd_info->newpwd == '.')
		set_append_path(info, cd_info, env_pwd);
	else
	{
		exit_status += add_env(info, ft_strdup("PWD"), getcwd(NULL, 0));
		exit_status += add_env(info, ft_strdup("OLDPWD"),
				ft_strdup_ns(env_pwd));
	}
	if (exit_status > 0)
		return (EXIT_FAILURE);
	if (cd_info->cd_type == e_home)
		printf("%s\n", *cd_info->home);
	if (cd_info->cd_type == e_oldpwd)
		printf("%s\n", *cd_info->oldpwd);
	return (EXIT_SUCCESS);
}

char	*lst_to_chr(t_list *head, char *newpwd)
{
	t_list	*temp_head;
	char	*cdpath;
	char	*temp;

	temp_head = head;
	while (head != NULL)
	{
		if (head == temp_head)
			cdpath = ft_strjoin("/", (char *)head->content);
		else
		{
			temp = cdpath;
			cdpath = ft_strjoin(cdpath, "/");
			free(temp);
			temp = cdpath;
			cdpath = ft_strjoin(cdpath, (char *)head->content);
			free(temp);
		}
		head = head->next;
	}
	free(newpwd);
	ft_lstclear(&temp_head, free);
	return (cdpath);
}

int	check_chdir(t_cd_info *cd_info)
{
	char	**split_pwd;
	t_list	*head;
	char	**temp_char_ptr;
	int		ret_chdir;

	ret_chdir = chdir(cd_info->newpwd);
	if (ret_chdir == 0)
		return (SUCCESS);
	split_pwd = ft_split(cd_info->newpwd, '/');
	head = NULL;
	temp_char_ptr = split_pwd;
	while (*split_pwd != NULL)
	{
		if (!is_same_str(*split_pwd, "..") && \
			!is_same_str(*(split_pwd + 1), ".."))
			ft_lstadd_back(&head, ft_lstnew(ft_strdup_ns(*split_pwd)));
		split_pwd++;
	}
	free_2d_alloc((void **)temp_char_ptr);
	cd_info->newpwd = lst_to_chr(head, cd_info->newpwd);
	ret_chdir = chdir(cd_info->newpwd);
	if (ret_chdir < 0)
		return (FAILURE);
	return (SUCCESS);
}

int	chdir_setpath(t_info *info, t_cd_info *cd_info, char **cmds)
{
	int	exit_dir;

	exit_dir = check_dir_exist(cd_info->newpwd, *cmds);
	if (exit_dir == SUCCESS)
	{
		chdir(cd_info->newpwd);
		set_path(info, cd_info);
		return (EXIT_SUCCESS);
	}
	if (exit_dir == EACCES)
	{
		if (check_chdir(cd_info) == SUCCESS)
			set_path(info, cd_info);
		else
		{
			ft_printf("minishell: cd: %s: Permission denied\n", *cmds);
			return (EXIT_FAILURE);
		}
	}
	return (exit_dir);
}
