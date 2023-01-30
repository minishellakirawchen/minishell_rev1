/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:45 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 22:25:34 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static void	move_to_next_exec_node(t_exec_list **exec_list_start_with_operator, int exit_status);

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

int	execute_execlist(t_exec_list **execlist_head, t_info *info)
{
	int			exit_status;
	t_exec_list	*exec_node;
	t_exec_list	*pipeline_node;
	bool		debug = true;

	if (!info || !execlist_head)
		return (FAILURE);
	exit_status = EXIT_SUCCESS;

	exec_node = *execlist_head;

	/* exec heredoc */
	if (execute_heredoc(execlist_head) == FAILURE)
		return (FAILURE);

	/* expand_var and execute_commands in pipeline_node unit */
	exec_node = *execlist_head;
	while (exec_node)
	{
		pipeline_node = exec_node;
		/* expansion */
		if (expand_var_and_create_commands_from_tokens(&pipeline_node, info) == FAILURE)
			return (FAILURE);

		if (debug)
		{
				/* vvvvv debug mode: print command_info vvvvv */
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
			/* ^^^^^ debug mode: print command_info ^^^^^ */
		}
		printf("\nvvvvv execute vvvvv\n");
		/* execution */
		exit_status = execute_pipeline(pipeline_node->pipeline_commands, info);
		printf("^^^^^^^^^^^^^^^^^^^  ");
		if (exit_status == PROCESS_ERROR)
			return (PROCESS_ERROR);
		/* get next pipeline node */
		exec_node = exec_node->next;
		move_to_next_exec_node(&exec_node, exit_status);
	}
//	debug_print_exec_list(info->execlist_head, "expansion");
	return (exit_status);
}

// operator -> pipeline -> operator -> ..
static void	move_to_next_exec_node(t_exec_list **exec_list_start_with_operator, int exit_status)
{
	t_exec_list	*next_operator_node;
	bool		debug = false;

	printf("exit_status:%d\n", exit_status);

	if (!exec_list_start_with_operator || !*exec_list_start_with_operator)
		return ;

	next_operator_node = *exec_list_start_with_operator;
	/* vvvvv debug mode: execute all node vvvvv */
	if (debug)
	{
		debug_print_exec_nodetype(next_operator_node);
		(*exec_list_start_with_operator) = (*exec_list_start_with_operator)->next;
		return ;
	}
	/* ^^^^^ debug mode: execute all node ^^^^^ */

	if ((next_operator_node->node_kind == e_node_semicolon)
	|| (next_operator_node->node_kind == e_node_and && exit_status == 0)
	|| (next_operator_node->node_kind == e_node_or && exit_status != 0))
	{
		(*exec_list_start_with_operator) = (*exec_list_start_with_operator)->next;
		return ;
	}
	//goto next semicolon->next
	while (*exec_list_start_with_operator && (*exec_list_start_with_operator)->node_kind != e_node_semicolon)
		(*exec_list_start_with_operator) = (*exec_list_start_with_operator)->next;
	if (*exec_list_start_with_operator)
		(*exec_list_start_with_operator) = (*exec_list_start_with_operator)->next;
}

