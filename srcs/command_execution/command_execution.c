/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:45 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 02:11:22 by wchen            ###   ########.fr       */
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
	char	*cmds1[20] = {"export", "z", NULL};
	exit_status = execute_builtin(info, cmds1);
	char	*cmds2[20] = {"export", "b=", NULL};
	exit_status = execute_builtin(info, cmds2);
	char	*cmds3[20] = {"export", "A=test", NULL};
	exit_status = execute_builtin(info, cmds3);
	printf("----------after-------------\n");
	char	*cmds4[20] = {"env", "test_dir", NULL};
	exit_status = execute_builtin(info, cmds4);
	char	*cmds5[20] = {"export", NULL};
	exit_status = execute_builtin(info, cmds5);
	char	*cmds6[20] = {"export", "Z", NULL};
	exit_status = execute_builtin(info, cmds6);
	char	*cmds7[20] = {"export", NULL};
	exit_status = execute_builtin(info, cmds7);
	char	*cmds8[20] = {"env", NULL};
	exit_status = execute_builtin(info, cmds8);
	// ft_env(info);
	printf("----------after-------------\n");
	//ft_execve();
	return (exit_status);
}
