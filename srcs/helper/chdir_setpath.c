/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir_setpath.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 12:39:43 by wchen             #+#    #+#             */
/*   Updated: 2023/01/31 01:30:56 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_append_path(t_info *info, t_cd_info *cd_info, char *env_pwd)
{
	char	*temp;
	int		exit_status;

	exit_status = 0;
	temp = cd_info->newpwd;
	cd_info->newpwd = ft_strjoin("/", cd_info->newpwd);
	exit_status += append_env(info, ft_strdup("PWD="),
			ft_strdup_ns(cd_info->newpwd));
	exit_status += add_env(info, ft_strdup("OLDPWD"), ft_strdup_ns(env_pwd));
	free(temp);
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
	return (EXIT_SUCCESS);
}

int	check_path(char *newpwd)
{
	char	**split_pwd;
	t_list	*head;
	char	*check_pwd;
	int		i;
	char	*temp;

	split_pwd = ft_split(newpwd, '/');
	head = NULL;
	while (*split_pwd != NULL)
	{
		if (is_same_str(*split_pwd, "..") == SUCCESS)
			ft_lstdelone(ft_lstlast(head), free);
		else
			ft_lstadd_back(&head, ft_lstnew(*split_pwd));
		split_pwd ++;
	}
	i = 0;
	while(head != NULL)
	{
		if (i ==  0)
			check_pwd = ft_strjoin("/", (char *)head->content);
		else
		{
			temp = check_pwd;
			check_pwd = ft_strjoin(check_pwd, "/");
			free (temp);
			temp = check_pwd;
			check_pwd = ft_strjoin(check_pwd, (char *)head->content);
			free (temp);
		}
		i ++;
		printf("node: %s\n", (char *)head->content);
		head = head->next;
	}
	printf("check_pwd : %s\n", check_pwd);fflush(stdout);
	return (FAILURE);

}

int	chdir_setpath(t_info *info, t_cd_info *cd_info, char **cmds)
{
	int		ret_chdir;

	if (check_dir_exist(cd_info->newpwd) == EACCES &&
		check_path(cd_info->newpwd) == FAILURE)
	{
		ft_printf("minishell: cd: %s: Permission denied\n", *cmds);
		return (EXIT_FAILURE);
	}
	ret_chdir = chdir(cd_info->newpwd);
	if (ret_chdir < 0)
	{
		ft_printf("minishell: cd: %s: No such file or directory\n", *cmds);
		return (EXIT_FAILURE);
	}
	else
		set_path(info, cd_info);
	if (cd_info->cd_type == e_home)
		printf("%s\n", *cd_info->home);
	if (cd_info->cd_type == e_oldpwd)
		printf("%s\n", *cd_info->oldpwd);
	return (EXIT_SUCCESS);
}
