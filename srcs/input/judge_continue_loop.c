/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   judge_continue_loop.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 10:10:51 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 10:10:51 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

bool	is_eof_exit(int process_exit_value)
{
	if (process_exit_value == BREAK)
	{
		ft_dprintf(STDERR_FILENO, "exit\n");
		return (true);
	}
	return (false);
}

bool	is_minishell_abort(int process_exit_value)
{
	if (process_exit_value == PROCESS_ERROR)
	{
		ft_dprintf(STDERR_FILENO, "minishell: abort\n");
		return (true);
	}
	return (false);
}
