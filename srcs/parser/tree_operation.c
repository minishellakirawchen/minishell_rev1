/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_operation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 09:19:29 by takira            #+#    #+#             */
/*   Updated: 2023/01/20 13:55:42 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "paeser.h"

t_tree	*get_last_node(t_tree *elem)
{
	if (!elem)
		return (NULL);
	while (elem->right)
		elem = elem->right;
	return (elem);
}

void	add_top_of_tree(t_tree **tree, t_tree *elem)
{
	if (!tree)
		return ;
	elem->left = NULL;
	elem->right = *tree;
	if (*tree)
		(*tree)->left = elem;
	*tree = elem;
}

void	add_bottom_of_tree(t_tree **tree, t_tree *elem)
{
	t_tree	*bottom_elem;

	if (!tree)
		return ;
	if (!*tree)
	{
		*tree = elem;
		elem->right = NULL;
		elem->left = NULL;
		return ;
	}
	bottom_elem = get_last_node(*tree);
	bottom_elem->right = elem;
	elem->left = bottom_elem;
	elem->right = NULL;
}

t_tree	*pop_from_top(t_tree **tree)
{
	t_tree	*top_elem;

	if (!tree || !*tree)
		return (NULL);
	top_elem = *tree;
	*tree = top_elem->right;
	if (top_elem->right)
		top_elem->right->left = NULL;
	top_elem->left = NULL;
	top_elem->right = NULL;
	return (top_elem);
}

t_tree	*pop_from_bottom(t_tree **tree)
{
	t_tree	*bottom_elem;

	if (!tree || !*tree)
		return (NULL);
	bottom_elem = get_last_node(*tree);
	if (bottom_elem->left)
		bottom_elem->left->right = NULL;
	bottom_elem->left = NULL;
	bottom_elem->right = NULL;
	return (bottom_elem);
}
