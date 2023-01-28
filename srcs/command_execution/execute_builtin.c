/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 15:35:04 by wchen             #+#    #+#             */
/*   Updated: 2023/01/28 17:58:05 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_info *info, char **cmds)
{
	if (is_same_str("env", cmds[0]))
		return (ft_env(info));
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
	return (EXIT_FAILURE);
}