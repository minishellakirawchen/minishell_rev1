/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:24 by takira            #+#    #+#             */
/*   Updated: 2023/02/07 12:56:13 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmds(char **cmds)
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

static int	cmd_to_ll(char **cmds)
{
	bool	is_strtoll_success;
	int		exit_status;
	char	*input_exit_num_chr;

	input_exit_num_chr = ft_strtrim(cmds[1], " ");
	if (!input_exit_num_chr)
		return (EXIT_FAILURE);
	exit_status = (int)(ft_strtoll(input_exit_num_chr,
				&is_strtoll_success) % 256);
	free(input_exit_num_chr);
	if (!is_strtoll_success)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n",
			(char *)cmds[1]);
		return (255);
	}
	return (exit_status);
}

int	ft_exit(t_info *info, char **cmds, bool in_pipe)
{
	int	exit_status;
	int	cmds_count;

	if (!info || !cmds)
		return (EXIT_FAILURE);
	if (!in_pipe)
		ft_dprintf(STDERR_FILENO, "exit\n");
	cmds_count = count_cmds(&cmds[1]);
	exit_status = cmd_to_ll(cmds);
	if (cmds_count > 1 && exit_status != 255)
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		return (EXIT_FAILURE);
	}
	if (cmds_count == 0)
		exit_status = EXIT_SUCCESS;
	exit_status %= 256;
	exit(exit_status);
}
