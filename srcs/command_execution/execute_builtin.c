/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 15:35:04 by wchen             #+#    #+#             */
/*   Updated: 2023/01/22 16:23:21 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_info *info, char **cmds)
{
	if (is_same_str("env", cmds[0]))
		return (ft_env(info));
	if (is_same_str("export", cmds[0]))
		return (ft_export(info, cmds));
	return (EXIT_FAILURE);
}
