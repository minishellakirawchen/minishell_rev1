/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_opendir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 10:55:05 by takira            #+#    #+#             */
/*   Updated: 2023/02/01 11:23:38 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int main(int argc, char **argv)
{
	DIR 			*dp;
	struct dirent	*dirp;

	if (argc != 2)
		return (0);
	dp = opendir(argv[1]);
	if (!dp)
	{
		printf("can't open %s\n", argv[1]);
		return (1);
	}

	dirp = readdir(dp);
	while (dirp)
	{
		printf("%s\n", dirp->d_name);
		dirp = readdir(dp);
	}
	closedir(dp);
	return (0);
}