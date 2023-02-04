/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_execlist_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 08:31:12 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 08:35:00 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_exec_list	*set_next_node(t_exec_list **next, t_exec_list *new_node);
static t_exec_list	*get_prev_node(t_exec_list **prev, t_exec_list *new_node);

t_exec_list	*create_execlist_node(\
t_node_kind kind, t_list_bdi *token_lst, t_exec_list **prev, t_exec_list **next)
{
	t_exec_list	*new_node;

	errno = 0;
	new_node = (t_exec_list *)malloc(sizeof(t_exec_list));
	if (!new_node)
		return (perror_ret_nullptr("malloc"));
	new_node->node_kind = kind;
	new_node->token_list_head = token_lst;
	new_node->pipeline_commands = NULL;
	new_node->token_type = e_init;
	new_node->prev = get_prev_node(prev, new_node);
	new_node->next = set_next_node(next, new_node);
	return (new_node);
}

static t_exec_list	*get_prev_node(t_exec_list **prev, t_exec_list *new_node)
{
	t_exec_list	*set_prev;

	set_prev = NULL;
	if (prev && *prev)
	{
		set_prev = *prev;
		(*prev)->next = new_node;
	}
	return (set_prev);
}

static t_exec_list	*set_next_node(t_exec_list **next, t_exec_list *new_node)
{
	t_exec_list	*set_next;

	set_next = NULL;
	if (next && *next)
	{
		set_next = *next;
		(*next)->prev = new_node;
	}
	return (set_next);
}
