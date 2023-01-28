/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:45 by takira            #+#    #+#             */
/*   Updated: 2023/01/28 22:34:40 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int command_execution(t_info *info)
{
	int	exit_status;

	//commad_list
	// char	*cmds1[20] = {"export", "CDPATH=/Users/weijuan/projects/42Tokyo/MINI_SHELL/minishell_rev1/test_dir/hoge", NULL};
	// exit_status = execute_builtin(info, cmds1);
	printf("----------before-------------\n");
	ft_env(info);
	printf("----------before-------------\n");
	char	*cmds[20] = {"cd", "./test_dir", NULL};
	exit_status = execute_builtin(info, cmds);
	printf("----------after-------------\n");
	ft_env(info);
	printf("----------after-------------\n");
	//ft_execve();
	return (exit_status);
}
