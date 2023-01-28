/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:53:07 by wchen             #+#    #+#             */
/*   Updated: 2023/01/28 21:16:20 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	perror_and_return_int(char *err, int exit_status)
{
	perror(err);
	return (exit_status);
}

int free_cdinfo_ret_status(t_cd_info *cd_info, int exit_status)
{
	if (cd_info->env_pwd != NULL)
		free(cd_info->env_pwd);
	if (cd_info->pwd != NULL)
		free(cd_info->pwd);
	if (cd_info->newpwd != NULL)
		free (cd_info->newpwd);
	free(cd_info);
	return (exit_status);
}
