/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 09:34:52 by takira            #+#    #+#             */
/*   Updated: 2023/01/21 11:45:36 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	main(void)
{
	extern char **environ;
	char *test_env[10] = {"hoge=hoge", NULL};
	char *cmds[10] = {"/usr/bin/env", NULL};

	printf("\n");

	execve(cmds[0], cmds, test_env);

	printf("finish\n");
	return(0);
}