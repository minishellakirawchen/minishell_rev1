/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dir_exist.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 22:37:33 by wchen             #+#    #+#             */
/*   Updated: 2023/02/01 20:34:26 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_dir_exist(char *tdir)
{
	DIR		*dir;
	int		err_no;

	err_no = SUCCESS;
	dir = opendir(tdir);
	if (ENOENT == errno)
		err_no = ENOENT;
	if (EACCES == errno)
		err_no = EACCES;
	if (dir)
		closedir(dir);
	return (err_no);
}
