/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:45 by takira            #+#    #+#             */
/*   Updated: 2023/01/25 21:36:52 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

int	execute_execlist(t_info *info)
{
	int			exit_status;
//	t_exec_list	*exec_node;

	if (!info || !info->execlist_head)
		return (FAILURE);
	exit_status = EXIT_SUCCESS;

	/*
	while (exec_node)
	{

		exec_node = exec_node->next;
	}
	 */

	return (exit_status);
}