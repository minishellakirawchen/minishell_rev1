/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:32:12 by takira            #+#    #+#             */
/*   Updated: 2023/02/03 14:58:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* FREE OK */
#include "command_execution.h"

int	execute_subshell(t_list_bdi **token_list, t_info *info)
{
	t_exec_list	*exec_list;
	int			exit_status;

	if (!token_list || !info)
		return (PROCESS_ERROR);

	// debug print //
	t_token_elem	*token = (*token_list)->content;
	int				subshell_depth = token->subshell_depth;
	dprintf(STDERR_FILENO, "\n ########## vvv execute subshell depth:%d vvv ##########\n", subshell_depth);
//	debug_print_tokens(*token_list, "subshell token");
	// // // // // //

	exec_list = NULL;
	if (parsing_token_list(token_list, &exec_list, info) == PROCESS_ERROR)
	{
		clear_exec_list(&exec_list);
		return (PROCESS_ERROR);
	}

//	debug_print_exec_list(exec_list, "subshell exec");

	exit_status = execute_execlist(&exec_list, info);

	clear_exec_list(&exec_list);
	dprintf(STDERR_FILENO, "\n ########## ^^^ execute subshell depth:%d ^^^ ##########\n\n", subshell_depth);
	return (exit_status);
}
