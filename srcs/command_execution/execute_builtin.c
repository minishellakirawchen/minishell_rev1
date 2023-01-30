/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 15:35:04 by wchen             #+#    #+#             */
/*   Updated: 2023/01/30 11:47:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

bool	is_single_builtin(t_list_bdi *pipeline_cmds_head)
{
	t_command_info	*command_info;

	command_info = pipeline_cmds_head->content;
	if (!command_info->commands)
		return (false);
	if (!is_builtin(command_info->commands))
		return (false);
	if (ft_lstsize_bdi(pipeline_cmds_head) > 1)
		return (false);
	if (command_info->redirect_list)
		return (false);
	return (true);
}

bool	is_builtin(char **cmds)
{
	if (!cmds || !*cmds)
		return (false);
	if (is_same_str("env", cmds[0]))
		return (true);
	if (is_same_str("export", cmds[0]))
		return (true);
	if (is_same_str("unset", cmds[0]))
		return (true);
	if (is_same_str("ft_echo", cmds[0])) //TODO:ft_echo->echo
		return (true);
	if (is_same_str("pwd", cmds[0]))
		return (true);
	if (is_same_str("cd", cmds[0]))
		return (true);
	if (is_same_str("exit", cmds[0]))
		return (true);
	return (false);
}
int	execute_builtin(t_info *info, char **cmds)
{
	if (is_same_str("env", cmds[0]))
		return (ft_env(info, cmds));
	if (is_same_str("export", cmds[0]))
		return (ft_export(info, cmds));
	if (is_same_str("unset", cmds[0]))
		return (ft_unset(info, cmds));
	if (is_same_str("echo", cmds[0]))
		return (ft_echo(cmds));
	if (is_same_str("pwd", cmds[0]))
		return (ft_pwd(info));
	if (is_same_str("cd", cmds[0]))
		return (ft_cd(info, cmds));
	if (is_same_str("exit", cmds[0]))
		return (ft_exit(info, cmds));
	return (EXIT_FAILURE);
}
