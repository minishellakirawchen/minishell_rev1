/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:24 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 14:06:38 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: input->handling
// ex) exit 1   -> echo $? = 1
// ex) exit 255 -> echo $? = 255
// ex) exit 256 -> echo $? = 0
// ex) exit a   -> error

// bash $> exit a b
// exit
// bash: exit: a: numeric argument required
// zsh  $> echo $?
// 255

// bash $> exit 1 a
// exit
// bash: exit: too many arguments
// bash $> echo $?
// 1

// bash $> exit 1 1
// exit
// bash: exit: too many arguments
// bash $> echo $?
// 1

// cmds = {exit, foo, bar, .., NULL}

// exit 2147483647 -> 255
// exit 2147483648 -> 0

// exit 9223372036854775807 -> 255
// exit 9223372036854775808 -> bash: exit: 9223372036854775808: numeric argument required

int count_cmds(char **cmds)
{
	int	count;

	count = 0;
	while(*cmds != NULL)
	{
		count ++;
		cmds ++;
	}
	return (count);
}

int	ft_exit(t_info *info, char **cmds)
{
	int				exit_status;
	bool			is_strtoll_success;
	int				cmds_count;

	if (!info || !cmds)
		return (EXIT_FAILURE);
	ft_dprintf(STDERR_FILENO, "exit");
	cmds_count = count_cmds(&cmds[1]);
	if (cmds_count > 1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments");
		return (EXIT_TOO_MANY_ARGS);
	}
	exit_status = EXIT_SUCCESS;
	if (cmds_count == 1)
	{
		exit_status = (int)(ft_strtoll((char *)cmds[1], &is_strtoll_success) % 256);
		if (!is_strtoll_success)
		{
			ft_dprintf(STDERR_FILENO, "minishell: exit: %s: numeric argument required", (char *)cmds[1]);
			exit (EXIT_NUMERIC_ARGS_REQUIRED);
		}
	}
	exit_status %= 256;
	exit(exit_status);
}
