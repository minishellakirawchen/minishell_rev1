/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:32:12 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 14:33:52 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

int	execute_subshell(t_list_bdi **token_list, t_info *info)
{
	t_exec_list	*exec_list;
	int			exit_status;

	if (!token_list || !info)
		return (PROCESS_ERROR);
	exec_list = NULL;
	if (parsing_token_list(token_list, &exec_list, info) == PROCESS_ERROR)
	{
		clear_exec_list(&exec_list);
		return (PROCESS_ERROR);
	}
	exit_status = execute_execlist(&exec_list, info);
	clear_exec_list(&exec_list);
	return (exit_status);
}
