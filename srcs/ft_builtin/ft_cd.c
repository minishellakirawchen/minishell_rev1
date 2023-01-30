/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 22:26:21 by wchen             #+#    #+#             */
/*   Updated: 2023/01/31 00:27:29 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*init_tdir(char *ref, char *cmd)
{
	ssize_t	ref_len;
	ssize_t	cmd_len;
	char	*trimed_cmd;
	char	*tdir;

	if (ref == NULL)
		return (ft_strdup_ns(cmd));
	ref_len = ft_strlen(ref);
	trimed_cmd = ft_strtrim(cmd, "/");
	if (!trimed_cmd)
		return (NULL);
	cmd_len = ft_strlen(trimed_cmd);
	tdir = ft_calloc(sizeof(char), ref_len + cmd_len + 2);
	if (!tdir)
		return (NULL);
	ft_strlcat(tdir, ref, ref_len + 1);
	ft_strlcat(tdir, "/", ref_len + 2);
	ft_strlcat(tdir, trimed_cmd, ref_len + cmd_len + 2);
	free_1d_alloc(trimed_cmd);
	return (tdir);
}

static char	*define_new_path(t_cd_info *cd_info, char *cmd)
{
	char	*tdir;
	char	**cdpaths;

	if (cd_info->cd_type == e_home)
		return (ft_strdup_ns(*(cd_info->home)));
	else if (cd_info->cd_type == e_absolute)
		return (ft_strdup_ns(cmd));
	else if (cd_info->cd_type == e_oldpwd)
		return (ft_strdup_ns(*(cd_info->oldpwd)));
	else if (cd_info->cd_type == e_relative)
		return (init_tdir(cd_info->pwd, cmd));
	else if (cd_info->cdpath != NULL)
	{
		cdpaths = ft_split(*(cd_info->cdpath), ':');
		while (*cdpaths != NULL)
		{
			tdir = init_tdir(*cdpaths, cmd);
			if (check_dir_exist(tdir) == SUCCESS)
				return (tdir);
			cdpaths++;
		}
	}
	return (init_tdir(cd_info->pwd, cmd));
}

static t_cd_info	*init_cd_info(t_info *info)
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
		if (check_dir_exist("./") == EACCES)
			cd_info->pwd = ft_strdup(*get_elem(info, "PWD"));
		else
		{
			ft_dprintf(STDERR_FILENO, "cd: error retrieving current directory: ");
			ft_dprintf(STDERR_FILENO, "getcwd: cannot access parent directories: ");
			ft_dprintf(STDERR_FILENO, "directories: No such file or directory ");
		}
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
	cd_info = init_cd_info(info);
	if (!cd_info)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	cd_info->cd_type = judge_cmd(*(++cmds));
	if (cd_error_handler(cd_info, cmds) == EXIT_FAILURE)
		return (free_cdinfo_ret_status(cd_info, EXIT_FAILURE));
	cd_info->newpwd = define_new_path(cd_info, *cmds);
	printf("-----debug-----\n");
	printf("new pwd : %s\n", cd_info->newpwd);
	printf("-----debug-----\n");
	if (!cd_info->newpwd)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	exit_status = chdir_setpath(info, cd_info, cmds);
	return (free_cdinfo_ret_status(cd_info, exit_status));
}
