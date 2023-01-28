/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:45 by takira            #+#    #+#             */
/*   Updated: 2023/01/28 21:10:06 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static void	move_to_next_exec_node(t_exec_list **exec_list, int exit_status);

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
 *
 *
 * bash 0$ echo hello a1=$a1 && export a1=a1dayo && echo hello a1=$a1
 * hello a1=
 * hello a1=a1dayo
 *
 *
 * pipeline_i = cmd_i1 | cmd_i2 | cmd_i3 .. cmd_in
 * 		pipeline_i : t_list_bdi
 * 		cmd_ij : t_command_info
 *
 * */

// pipelien_commands cmd1 | cmd2 | cmd3

int	execute_execlist(t_info *info)
{
	int			exit_status;
	t_exec_list	*exec_node;
	t_exec_list	*pipeline_node;

	if (!info || !info->execlist_head)
		return (FAILURE);
	exit_status = EXIT_SUCCESS;

	printf("\n***** execute *****\n");
	printf("\nvvvvvvvvvvvvvvvvvvv\n");
	exec_node = info->execlist_head;
	while (exec_node)
	{
		pipeline_node = exec_node;
		/* expansion */
		if (expand_variable(&pipeline_node, info) == FAILURE)
			return (FAILURE);

		/* execution */
		exit_status = execute_pipeline(pipeline_node->pipeline_commands, info->envlist_head);
		exec_node = exec_node->next;
		move_to_next_exec_node(&exec_node, exit_status);
	}
	printf("^^^^^^^^^^^^^^^^^^^\n");

//	debug_print_exec_list(info->execlist_head, "expansion");

	return (exit_status);
}

// operator -> pipeline -> operator -> ..
static void	move_to_next_exec_node(t_exec_list **exec_list, int exit_status)
{
	t_exec_list	*next_operator_node;
	bool		debug = true;

	printf("exit_status:%d\n", exit_status);
	if (!exec_list || !*exec_list)
		return ;

	next_operator_node = *exec_list;
	/* vvvvv debug mode: execute all node vvvvv */
	if (debug)
	{
		debug_print_exec_nodetype(next_operator_node);
		(*exec_list) = (*exec_list)->next;
		return ;
	}
	/* ^^^^^ debug mode: execute all node ^^^^^ */

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
