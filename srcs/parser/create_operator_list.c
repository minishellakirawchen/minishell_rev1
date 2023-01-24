/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_exec_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:06:20 by takira            #+#    #+#             */
/*   Updated: 2023/01/24 17:10:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "paeser.h"

static t_exec_list *create_execlist_node(t_node_kind kind, t_list *token_head, t_exec_list **prev, t_exec_list **next);

// echo hello | echo world && cat Makefile | grep a || (cd /bin && ls) > out; cat <infile;
// A && B || C ; D
// [root]
//   |
//  [pipeline_commands]:echo hello | echo world
//  [&&]
//  [pipeline_commands]:echo hello | echo world

// 初めはpipelineを作る
// operatorがきたらpipeline->operator

t_exec_list	*create_operator_list(t_list **tokenlist_head)
{
	t_list			*popped_node;
	t_token_elem	*token_elem;

	t_exec_list		*exec_head;
	t_exec_list		*pipeline_node;
	t_exec_list		*operator_node;
	ssize_t			subshell_depth;

	if (!tokenlist_head || !*tokenlist_head)
		return (NULL);
	exec_head = create_execlist_node(e_node_head, NULL, NULL, NULL);
	pipeline_node = create_execlist_node(e_node_pipeline, NULL, &exec_head, NULL);

	if (!exec_head || !pipeline_node)
		return (NULL); //TODO:all free

	token_elem = (*tokenlist_head)->content;
	subshell_depth = token_elem->depth;
	while (*tokenlist_head)
	{
		popped_node = ft_lstpop(&(*tokenlist_head));
		token_elem = popped_node->content;
		if (!(is_tokentype_operator(token_elem->type) && token_elem->depth == subshell_depth))
			ft_lstadd_back(&(pipeline_node->token_list_head), popped_node);
		else
		{
			operator_node = create_execlist_node(e_node_operator, NULL, &pipeline_node, NULL);
			if (!operator_node)
				return (perror_ret_nullptr("malloc")); // TODO:all free
			operator_node->token_type = token_elem->type; //for debug print
			ft_lstdelone(popped_node, free_token_elem);
			popped_node = NULL;
			if (*tokenlist_head)
			{
				pipeline_node = create_execlist_node(e_node_pipeline, NULL, &operator_node, NULL);
				if (!pipeline_node)
					return (perror_ret_nullptr("malloc")); // TODO:all free
			}
		}
	}
	return (exec_head);
}


t_exec_list *create_execlist_node(t_node_kind kind, t_list *token_head, t_exec_list **prev, t_exec_list **next)
{
	t_exec_list	*new_node;
	t_exec_list	*set_prev;
	t_exec_list	*set_next;

	errno = 0;
	new_node = (t_exec_list *)malloc(sizeof(t_exec_list));
	if (!new_node)
		return (perror_ret_nullptr("malloc"));
	set_prev = NULL;
	set_next = NULL;
	if (prev && *prev)
	{
		set_prev = *prev;
		(*prev)->next = new_node;
	}
	if (next && *next)
	{
		set_next = *next;
		(*next)->prev = new_node;
	}
	new_node->node_kind = kind;
	new_node->token_list_head = token_head;
	new_node->pipeline_commands = NULL;
	new_node->token_type = e_init; //tmp
	new_node->prev = set_prev;
	new_node->next = set_next;
	return (new_node);
}
