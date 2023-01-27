/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 18:28:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 20:03:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "paeser.h"

void	tree_clear(t_exec_list **stk)
{
	t_exec_list	*right;

	if (!stk)
		return ;
	while (*stk)
	{
		right = (*stk)->next;
		free(*stk);
		*stk = right;
	}
	*stk = NULL;
}

size_t	get_tree_size(t_exec_list *stk)
{
	size_t	size;

	size = 0;
	while (stk)
	{
		stk = stk->next;
		size++;
	}
	return (size);
}
