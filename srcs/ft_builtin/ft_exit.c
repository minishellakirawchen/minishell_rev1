/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:24 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 15:26:45 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmds(char **cmds)
{
	int	count;

	count = 0;
	while (*cmds != NULL)
	{
		count++;
		cmds++;
	}
	return (count);
}

int	cmd_to_ll(char **cmds)
{
	bool	is_strtoll_success;
	int		exit_status;

	exit_status = (int)(ft_strtoll((char *)cmds[1], &is_strtoll_success) % 256);
	if (!is_strtoll_success)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", (char *)cmds[1]);
		exit(EXIT_NUMERIC_ARGS_REQUIRED);
	}
	return (exit_status);
}

int	ft_exit(t_info *info, char **cmds)
{
	int		exit_status;
	int		cmds_count;

	if (!info || !cmds)
		return (EXIT_FAILURE);
	ft_dprintf(STDERR_FILENO, "exit\n");
	cmds_count = count_cmds(&cmds[1]);
	if (cmds_count > 1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		return (EXIT_TOO_MANY_ARGS);
	}
	exit_status = EXIT_SUCCESS;
	if (cmds_count == 1)
		exit_status = cmd_to_ll(cmds);
	exit_status %= 256;
	exit(exit_status);
}
