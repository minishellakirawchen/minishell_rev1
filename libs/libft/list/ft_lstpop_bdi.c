/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpop_bdi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 09:50:31 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:23:31 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_list.h"

t_list_bdi	*ft_lstpop_bdi(t_list_bdi **lst)
{
	t_list_bdi	*popped_lst;

	if (!lst || !*lst)
		return (NULL);
	popped_lst = *lst;
	if (popped_lst->prev)
		popped_lst->prev = *lst;
	*lst = popped_lst->next;
	popped_lst->prev = NULL;
	popped_lst->next = NULL;
	return (popped_lst);
}
