/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir_setpath.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 12:39:43 by wchen             #+#    #+#             */
/*   Updated: 2023/01/29 12:42:56 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_path(t_info *info, t_cd_info *cd_info)
{
	int	exit_status;

	exit_status = 0;
	if (*cd_info->newpwd == '.')
	{
		exit_status += append_env(info, ft_strdup("PWD="),
				ft_strdup(cd_info->newpwd));
		exit_status += add_env(info, ft_strdup("OLDPWD"),
				ft_strdup(cd_info->env_pwd));
	}
	else
	{
		exit_status += add_env(info, ft_strdup("PWD"), getcwd(NULL, 0));
		exit_status += add_env(info, ft_strdup("OLDPWD"),
				ft_strdup(cd_info->env_pwd));
	}
	if (exit_status > 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	chdir_setpath(t_info *info, t_cd_info *cd_info, char **cmds)
{
	int		ret_chdir;

	if (check_dir_exist(cd_info->newpwd) == EACCES)
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
	return (EXIT_SUCCESS);
}
