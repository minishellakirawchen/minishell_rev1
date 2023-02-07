/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_new_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 18:22:00 by wchen             #+#    #+#             */
/*   Updated: 2023/02/07 19:28:56 by wchen            ###   ########.fr       */
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

static char	*judge_cdpath(t_cd_info *cd_info, char *cmd)
{
	char	*tdir;
	char	**temp;
	char	**cdpaths;

	cdpaths = ft_split(*(cd_info->cdpath), ':');
	temp = cdpaths;
	while (*cdpaths != NULL)
	{
		tdir = init_tdir(*cdpaths, cmd);
		if (check_dir_exist(tdir, cmd, 0) == SUCCESS)
		{
			temp = (char **)free_2d_alloc((void **)temp);
			return (tdir);
		}
		tdir = free_1d_alloc(tdir);
		cdpaths++;
	}
	temp = (char **)free_2d_alloc((void **)temp);
	return (NULL);
}

char	*define_new_path(t_cd_info *cd_info, char *cmd)
{
	char	*tdir;

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
		tdir = judge_cdpath(cd_info, cmd);
		if (tdir != NULL)
			return (tdir);
	}
	return (init_tdir(cd_info->pwd, cmd));
}
