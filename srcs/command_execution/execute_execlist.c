/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:45 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 14:16:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

// operator -> pipeline -> operator -> ..
static void	move_to_next_exec_node(\
t_exec_list **exec_lst_start_with_ope, int exit_status)
{
	t_exec_list	*next_operator_node;

	if (!exec_lst_start_with_ope || !*exec_lst_start_with_ope)
		return ;
	next_operator_node = *exec_lst_start_with_ope;
	if ((next_operator_node->node_kind == e_node_semicolon)
		|| (next_operator_node->node_kind == e_node_and && exit_status == 0)
		|| (next_operator_node->node_kind == e_node_or && exit_status != 0))
	{
		(*exec_lst_start_with_ope) = (*exec_lst_start_with_ope)->next;
		return ;
	}
	while (*exec_lst_start_with_ope \
	&& (*exec_lst_start_with_ope)->node_kind != e_node_semicolon)
		(*exec_lst_start_with_ope) = (*exec_lst_start_with_ope)->next;
	if (*exec_lst_start_with_ope)
		(*exec_lst_start_with_ope) = (*exec_lst_start_with_ope)->next;
}

/*
 * pipelie1 || pipeline2 || .. || pipeline_n; pipeline_n+1 &&
 *  ^ EXEC      ^ not exec         ^not exec    ^ nextEXEC
 *     ^ if false >_____________________________|
 *
 * pipeline_i = cmd_i1 | cmd_i2 | cmd_i3 .. cmd_in
 * 		pipeline_i : t_list_bdi
 * 		cmd_ij : t_command_info
 */
int	execute_execlist(t_exec_list **execlist_head, t_info *info)
{
	int			exit_status;
	t_exec_list	*exec_node;
	t_exec_list	*pipeline_node;

	if (!info || !execlist_head)
		return (FAILURE);
	exit_status = EXIT_SUCCESS;
	exec_node = *execlist_head;
	if (execute_heredoc(execlist_head) == FAILURE)
		return (PROCESS_ERROR);
	exec_node = *execlist_head;
	while (exec_node)
	{
		pipeline_node = exec_node;
		if (expand_var_and_create_cmds_from_tokens(\
		&pipeline_node, info) == FAILURE)
			return (PROCESS_ERROR);
		exit_status = execute_pipeline(pipeline_node->pipeline_commands, info);
		if (exit_status == PROCESS_ERROR)
			return (PROCESS_ERROR);
		exec_node = exec_node->next;
		move_to_next_exec_node(&exec_node, exit_status);
	}
	return (exit_status);
}

/*

printf("---------- after expand, before execute ----------\n");
t_list_bdi *pipeline_cmds_node = pipeline_node->pipeline_commands;
while (pipeline_cmds_node)
{
t_command_info *command_info = pipeline_cmds_node->content;
// tmp print
debug_print_command_info(command_info);
pipeline_cmds_node = pipeline_cmds_node->next;
if (pipeline_cmds_node)
ft_dprintf(STDERR_FILENO, "       v [pipe:|] v\n");
}
printf("--------------------------------------------------\n");

 */