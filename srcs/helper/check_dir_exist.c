/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dir_exist.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 22:37:33 by wchen             #+#    #+#             */
/*   Updated: 2023/01/28 22:39:36 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_dir_exist(char *tdir)
{
	DIR		*dir;

	dir = opendir(tdir);
	if (ENOENT == errno)
		return (FAILURE);
	if (dir)
		closedir(dir);
	return (SUCCESS);
}
