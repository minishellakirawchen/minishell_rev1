/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_exec_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:06:20 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 17:40:57 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_exec_list *create_execlist_node(t_node_kind kind, t_list_bdi *token_head, t_exec_list **prev, t_exec_list **next);

// echo hello | echo world && cat Makefile | grep a || (cd /bin && ls) > out; cat <infile;
// A && B || C ; D
// [root]
//   |
//  [pipeline_commands]:echo hello | echo world
//  [&&]
//  [pipeline_commands]:echo hello | echo world

// 初めはpipelineを作る
// operatorがきたらpipeline->operator

static bool	is_pipeline_token(t_token_elem *token_elem, ssize_t	subshell_depth)
{
	if (!token_elem)
		return (false);
	if (!(is_tokentype_operator(token_elem->type) && token_elem->subshell_depth == subshell_depth))
		return (true);
	return (false);
}

t_exec_list	*create_execlist_operator_node(t_exec_list **prev_pipeline, t_list_bdi **popped_node, t_token_elem *token_elem)
{
	t_exec_list		*operator_node;
	t_node_kind		node_kind;

	if (!popped_node || !*popped_node)
		return (NULL);
	if (token_elem->type == e_semicolon)
		node_kind = e_node_semicolon;
	else if (token_elem->type == e_ope_and)
		node_kind = e_node_and;
	else
		node_kind = e_node_or;
	operator_node = create_execlist_node(node_kind, NULL, prev_pipeline, NULL);
	if (!operator_node)
		return (perror_ret_nullptr("malloc"));
	operator_node->token_type = token_elem->type; //for debug print
	return (operator_node);
}


void	delete_operator_token(t_list_bdi **operator_token)
{
	if (!operator_token || !*operator_token)
		return ;
	ft_lstdelone_bdi(operator_token, free_token_elem);
	*operator_token = NULL;
}

int handle_each_token(t_list_bdi **tokenlist, t_exec_list *pipeline_node, t_exec_list **operator_node, ssize_t subshell_depth)
{
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;

	popped_node = ft_lstpop_bdi(tokenlist);
	token_elem = popped_node->content;
	if (is_pipeline_token(token_elem, subshell_depth))
	{
		ft_lstadd_back_bdi(&(pipeline_node->token_list_head), popped_node);
		return (CONTINUE) ;
	}
	if (!operator_node)
		return (FAILURE);
	*operator_node = create_execlist_operator_node(&pipeline_node, &popped_node, token_elem);
	if (!*operator_node)
		return (perror_ret_int("malloc", FAILURE)); // TODO:all free
	delete_operator_token(&popped_node);
	if (!(*tokenlist))
	{
		(*operator_node)->next = NULL;
		return (BREAK);
	}
	return (SUCCESS);
}

/* create and connect t_exec_list, node type is pipeline and operator */
/* pipeline->operator->pipeline->... */

int	create_operator_list(t_list_bdi **tokenlist_head, t_exec_list **execlist_head)
{
	t_token_elem	*token_elem;
	int				subshell_depth;
	t_exec_list		*pipeline_node;
	t_exec_list		*operator_node;
	int				handle_result;

	errno = 0;
	if (!tokenlist_head || !*tokenlist_head || !execlist_head)
		return (FAILURE);
	*execlist_head = create_execlist_node(e_node_pipeline, NULL, NULL, NULL);
	if (!*execlist_head)
		return (FAILURE); //TODO:all free
	token_elem = (*tokenlist_head)->content;
	subshell_depth = token_elem->subshell_depth;
	if (token_elem->type == e_subshell_start)
		subshell_depth--;
	pipeline_node = *execlist_head;
	while (*tokenlist_head)
	{
		handle_result = handle_each_token(tokenlist_head, pipeline_node, &operator_node, subshell_depth);
		if (handle_result == CONTINUE)
			continue ;
		if (handle_result == BREAK)
			break ;
		if (handle_result == FAILURE)
			return (FAILURE);
		pipeline_node = create_execlist_node(e_node_pipeline, NULL, &operator_node, NULL);
		if (!pipeline_node)
			return (perror_ret_int("malloc", FAILURE)); // TODO:all free
	}
	return (SUCCESS);
}

t_exec_list *create_execlist_node(t_node_kind kind, t_list_bdi *token_head, t_exec_list **prev, t_exec_list **next)
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
