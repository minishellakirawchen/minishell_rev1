/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_operation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 09:19:29 by takira            #+#    #+#             */
/*   Updated: 2023/01/22 15:39:55 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_exec_list	*get_last_node(t_exec_list *node)
{
	if (!node)
		return (NULL);
	while (node->next)
		node = node->next;
	return (node);
}

void	add_top_of_tree(t_exec_list **tree, t_exec_list *add_elem)
{
	if (!tree)
		return ;
	add_elem->prev = NULL;
	add_elem->next = *tree;
	if (*tree)
		(*tree)->prev = add_elem;
	*tree = add_elem;
}

void	add_bottom_of_tree(t_exec_list **tree, t_exec_list *add_elem)
{
	t_exec_list	*bottom_elem;

	if (!tree)
		return ;
	if (!*tree)
	{
		*tree = add_elem;
		add_elem->next = NULL;
		add_elem->prev = NULL;
		return ;
	}
	bottom_elem = get_last_node(*tree);
	bottom_elem->next = add_elem;
	add_elem->prev = bottom_elem;
	add_elem->next = NULL;
}

t_exec_list	*pop_from_top(t_exec_list **tree)
{
	t_exec_list	*top_elem;

	if (!tree || !*tree)
		return (NULL);
	top_elem = *tree;
	*tree = top_elem->next;
	if (top_elem->next)
		top_elem->next->prev = NULL;
	top_elem->prev = NULL;
	top_elem->next = NULL;
	return (top_elem);
}

t_exec_list	*pop_from_bottom(t_exec_list **tree)
{
	t_exec_list	*bottom_elem;

	if (!tree || !*tree)
		return (NULL);
	bottom_elem = get_last_node(*tree);
	if (bottom_elem->prev)
		bottom_elem->prev->next = NULL;
	bottom_elem->prev = NULL;
	bottom_elem->next = NULL;
	return (bottom_elem);
}
