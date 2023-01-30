/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:32:12 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 16:10:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

int	execute_subshell(t_list_bdi **token_list, t_info *info)
{
	t_exec_list	*exec_list;
	int			exit_status;

	dprintf(STDERR_FILENO, "\n ########## vvv execute subshell vvv ##########\n");
	exec_list = NULL;
	if (parsing_token_list(token_list, &exec_list, info) == FAILURE)
		exit (PROCESS_ERROR);

	debug_print_exec_list(exec_list, "subshell");

	exit_status = execute_execlist(&exec_list, info);

	clear_exec_list(&exec_list);
	dprintf(STDERR_FILENO, "\n ########## ^^^ execute subshell ^^^ ##########\n\n");
	exit (exit_status);
}
