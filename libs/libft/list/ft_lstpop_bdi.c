/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpop_bdi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 09:50:31 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 14:00:55 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_list.h"

//               *lst------v *lst
// [prev_node]->[node]->[next_node]
// prev_node->next = next_node
// next_node->prev = prev_node
t_list_bdi	*ft_lstpop_bdi(t_list_bdi **lst)
{
	t_list_bdi	*popped_node;

	if (!lst || !*lst)
		return (NULL);
	popped_node = *lst;
	*lst = popped_node->next;
	if (popped_node->prev)
		popped_node->prev->next = *lst;
	if (*lst)
		(*lst)->prev = popped_node->prev;
	popped_node->prev = NULL;
	popped_node->next = NULL;
	return (popped_node);
}
