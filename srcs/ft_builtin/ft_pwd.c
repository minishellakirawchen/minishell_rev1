/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 22:13:40 by wchen             #+#    #+#             */
/*   Updated: 2023/01/27 22:49:38 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_info *info)
{
	char	**path;

	if (!info)
		return (EXIT_FAILURE);
	path = get_elem(info, "PWD");
	ft_printf("%s\n", *path);
	return (EXIT_SUCCESS);
}
