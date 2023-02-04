/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:53:07 by wchen             #+#    #+#             */
/*   Updated: 2023/02/02 16:29:10 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	perror_and_return_int(char *err, int exit_status)
{
	perror(err);
	return (exit_status);
}

int	free_cdinfo_ret_status(t_cd_info *cd_info, int exit_status)
{
	if (cd_info->pwd != NULL)
		free(cd_info->pwd);
	if (cd_info->newpwd != NULL)
		free(cd_info->newpwd);
	free(cd_info);
	return (exit_status);
}

int	cd_error_handler(t_cd_info *cd_info, char **cmds)
{
	if (cd_info->cd_type == e_opt_error)
	{
		ft_printf("minishell: cd: %c%c: invalid option\n", (*cmds)[0],
			(*cmds)[1]);
		return (EXIT_FAILURE);
	}
	else if (cd_info->cd_type == e_oldpwd &&
			(cd_info->oldpwd == NULL || *(cd_info->oldpwd) == NULL))
	{
		ft_printf("minishell: cd: OLDPWD not set\n");
		return (EXIT_FAILURE);
	}
	else if (cd_info->cd_type == e_home && cd_info->home == NULL)
	{
		ft_printf("minishell: cd: HOME not set\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
