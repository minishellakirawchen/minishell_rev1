/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_exec_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:06:20 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 08:44:44 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_exec_list	*create_execlist_operator_node(\
t_exec_list **prev_pipe, t_list_bdi **popped, t_token_elem *tok_elem)
{
	t_exec_list		*operator_node;
	t_node_kind		node_kind;

	if (!popped || !*popped)
		return (NULL);
	if (tok_elem->type == e_semicolon)
		node_kind = e_node_semicolon;
	else if (tok_elem->type == e_ope_and)
		node_kind = e_node_and;
	else
		node_kind = e_node_or;
	operator_node = create_execlist_node(node_kind, NULL, prev_pipe, NULL);
	if (!operator_node)
		return (perror_ret_nullptr("malloc"));
	operator_node->token_type = tok_elem->type;
	return (operator_node);
}

static int	handle_each_token(t_list_bdi **tokenlist, \
t_exec_list *pipeline_node, t_exec_list **operator_node, ssize_t subshell_depth)
{
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;

	popped_node = ft_lstpop_bdi(tokenlist);
	token_elem = popped_node->content;
	if (is_pipeline_token(token_elem, subshell_depth))
	{
		ft_lstadd_back_bdi(&(pipeline_node->token_list_head), popped_node);
		return (CONTINUE);
	}
	if (!operator_node)
		return (FAILURE);
	*operator_node = create_execlist_operator_node(\
	&pipeline_node, &popped_node, token_elem);
	if (!*operator_node)
		return (perror_ret_int("malloc", FAILURE));
	delete_operator_token(&popped_node);
	if (!(*tokenlist))
	{
		(*operator_node)->next = NULL;
		return (BREAK);
	}
	return (SUCCESS);
}

static int	setting_params(t_list_bdi **tokenlst_head, \
t_exec_list **execlst_head, t_exec_list **pipeline, int *subshell_depth)
{
	t_token_elem	*token_elem;

	if (!tokenlst_head || !*tokenlst_head || !execlst_head || !pipeline)
		return (FAILURE);
	*execlst_head = create_execlist_node(e_node_pipeline, NULL, NULL, NULL);
	if (!*execlst_head)
		return (FAILURE);
	token_elem = (*tokenlst_head)->content;
	*subshell_depth = token_elem->subshell_depth;
	if (token_elem->type == e_subshell_start)
		*subshell_depth -= 1;
	*pipeline = *execlst_head;
	return (SUCCESS);
}

/* create and connect t_exec_list, node type is pipeline and operator */
/* pipeline->operator->pipeline->... */
int	create_operator_list(t_list_bdi **tokenlst_head, t_exec_list **execlst_head)
{
	int				subshell_depth;
	int				handle_result;
	t_exec_list		*pipeline;
	t_exec_list		*operator;

	if (setting_params(\
	tokenlst_head, execlst_head, &pipeline, &subshell_depth) == FAILURE)
		return (FAILURE);
	while (*tokenlst_head)
	{
		handle_result = handle_each_token(\
		tokenlst_head, pipeline, &operator, subshell_depth);
		if (handle_result == CONTINUE)
			continue ;
		if (handle_result == BREAK)
			break ;
		if (handle_result == FAILURE)
			return (FAILURE);
		pipeline = create_execlist_node(e_node_pipeline, NULL, &operator, NULL);
		if (!pipeline)
			return (perror_ret_int("malloc", FAILURE));
	}
	return (SUCCESS);
}
