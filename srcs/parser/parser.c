/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:48 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 16:46:36 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* order of execute : heredoc -> redirect -> command */
/* order of expand  : heredoc = redirect = command */
/* target of expand : heredoc:content, redirect:filename, command:word */

/* create pipeline command list in operator list for command execution
 * and redirect list for redirection from tokens */
/* token list -> [operator_list]->[pipeline_command_list]
 *               [redirect_list]
 */

int	parsing_token_list(
		t_list_bdi **tokenlist_head, t_exec_list **execlist_head, t_info *info)
{
	if (!tokenlist_head || !execlist_head || !info)
		return (PROCESS_ERROR);
	debug_print_tokens(*tokenlist_head, "init");


	if (create_operator_list(tokenlist_head, execlist_head) == FAILURE)
		return (PROCESS_ERROR);
	debug_print_exec_list(*execlist_head, "create operator list");

	if (create_command_list(execlist_head) == FAILURE)
		return (PROCESS_ERROR);
	debug_print_exec_list(*execlist_head, "create command list");

	if (create_redirect_list(execlist_head, info) == FAILURE)
		return (PROCESS_ERROR);
	debug_print_exec_list(*execlist_head, "parsing fin");
	return (EXIT_SUCCESS);
}
//	debug_print_exec_list(*execlist_head, "parsing fin");
//	debug_print_exec_list(*execlist_head, "operator_list");
//	delete_last_semicolon_node(execlist_head);
//	debug_print_exec_list(info->execlist_head, "delete last ;");
