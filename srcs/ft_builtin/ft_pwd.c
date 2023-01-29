/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 22:13:40 by wchen             #+#    #+#             */
/*   Updated: 2023/01/28 22:09:58 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_info *info)
{
	char	**env_pwd;
	char	*cwd_pwd;

	if (!info)
		return (EXIT_FAILURE);
	env_pwd = get_elem(info, "PWD");
	if (env_pwd != NULL)
		ft_printf("%s\n", *env_pwd);
	else
	{
		cwd_pwd = getcwd(NULL, 0);
		if (!cwd_pwd)
			return (perror_and_return_int("getcwd", EXIT_FAILURE));
		else
			ft_printf("%s\n", cwd_pwd);
		free (cwd_pwd);
	}
	return (EXIT_SUCCESS);
}
