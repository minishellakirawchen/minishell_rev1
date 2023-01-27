/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:45 by takira            #+#    #+#             */
/*   Updated: 2023/01/27 22:51:37 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

/*
 * pipeline_node
 * pipeline_node &&
 * pipeline_node ||
 *
 * pipelie1 || pipeline2 || .. || pipeline_n; pipeline_n+1 &&
 *  ^ false     ^ not execu        ^not exec   ^ EXEC
 *
 * exec to next ; or NULL
 * if next_operator == &&
 * if next_operator_ == ||
 * */

int	execute_pipeline(t_list_bdi *pipeline_commands)
{
	int				exit_status;
	t_command_info	*command_info;

	if (!pipeline_commands)
		return (FAILURE);
	command_info = pipeline_commands->content;

	// exec redirect
	// if subshell
	//   exit_status = execute parsing->execute TODO:create function
	// else
	//   exit_status = execute pipeline
	return (exit_status);
}

// operator -> pipeline ->..
void	move_to_next_exec_node(t_exec_list **exec_list, int exit_status)
{
	t_exec_list	*next_operator_node;

	if (!exec_list || !*exec_list)
		return ;
	next_operator_node = *exec_list;
	if ((next_operator_node->node_kind == e_node_semicolon)
	|| (next_operator_node->node_kind == e_node_and && exit_status == 0)
	|| (next_operator_node->node_kind == e_node_or && exit_status != 0))
	{
		(*exec_list) = (*exec_list)->next;
		return ;
	}
	while (*exec_list && (*exec_list)->node_kind != e_node_semicolon)
		(*exec_list) = (*exec_list)->next;
}

int	execute_execlist(t_info *info)
{
	int			exit_status;
	t_exec_list	*exec_node;
	t_exec_list	*pipeline_node;

	if (!info || !info->execlist_head)
		return (FAILURE);
	exit_status = EXIT_SUCCESS;

	exec_node = info->execlist_head;
	while (exec_node)
	{
		pipeline_node = exec_node;
		if (expand_variable(&pipeline_node, info) == FAILURE)
			return (FAILURE);
		exit_status = execute_pipeline(pipeline_node->pipeline_commands);
		exec_node = exec_node->next;
		move_to_next_exec_node(&exec_node, exit_status);

		if (expand_variable(&exec_node, info) == FAILURE)
			return (FAILURE);
		exec_node = exec_node->next;
	}
	debug_print_exec_list(info->execlist_head, "expansion");

	return (exit_status);
}