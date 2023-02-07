/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 22:26:21 by wchen             #+#    #+#             */
/*   Updated: 2023/02/07 19:31:23 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_err_message(void)
{
	ft_dprintf(STDERR_FILENO, "cd: error retrieving current directory: ");
	ft_dprintf(STDERR_FILENO, "getcwd: cannot access parent directories: ");
	ft_dprintf(STDERR_FILENO, "directories: No such file or directory \n");
}

static t_cd_info	*init_cd_info(t_info *info, char *cmd)
{
	t_cd_info	*cd_info;

	cd_info = malloc(sizeof(t_cd_info));
	if (!cd_info)
		return (NULL);
	cd_info->cd_type = e_cd_init;
	cd_info->home = get_elem(info, "HOME");
	cd_info->cdpath = get_elem(info, "CDPATH");
	cd_info->env_pwd = get_elem(info, "PWD");
	cd_info->pwd = getcwd(NULL, 0);
	if (cd_info->pwd == NULL)
	{
		if (check_dir_exist("./", cmd, 1) == EACCES)
			cd_info->pwd = ft_strdup(*get_elem(info, "PWD"));
		else
			print_err_message();
	}
	cd_info->oldpwd = get_elem(info, "OLDPWD");
	cd_info->newpwd = NULL;
	return (cd_info);
}

int	ft_cd(t_info *info, char **cmds)
{
	int			exit_status;
	t_cd_info	*cd_info;

	exit_status = EXIT_SUCCESS;
	if (!info || !cmds)
		return (EXIT_FAILURE);
	cd_info = init_cd_info(info, *cmds);
	if (!cd_info)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	cd_info->cd_type = judge_cmd(*(++cmds));
	if (cd_error_handler(cd_info, cmds) == EXIT_FAILURE)
		return (free_cdinfo_ret_status(cd_info, EXIT_FAILURE));
	cd_info->newpwd = define_new_path(cd_info, *cmds);
	if (!cd_info->newpwd)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	exit_status = chdir_setpath(info, cd_info, cmds);
	return (free_cdinfo_ret_status(cd_info, exit_status));
}
