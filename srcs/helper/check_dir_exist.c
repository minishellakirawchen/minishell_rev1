/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dir_exist.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 22:37:33 by wchen             #+#    #+#             */
/*   Updated: 2023/01/29 12:28:08 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_dir_exist(char *tdir)
{
	DIR		*dir;

	dir = opendir(tdir);
	if (ENOENT == errno)
		return (ENOENT);
	if (EACCES == errno)
		return (EACCES);
	if (dir)
		closedir(dir);
	return (SUCCESS);
}
