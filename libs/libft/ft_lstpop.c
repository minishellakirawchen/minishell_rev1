/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 09:50:31 by takira            #+#    #+#             */
/*   Updated: 2023/01/24 19:26:26 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

t_list	*ft_lstpop(t_list **lst)
{
	t_list	*popped_lst;

	if (!lst || !*lst)
		return (NULL);
	popped_lst = *lst;
	*lst = popped_lst->next;
	popped_lst->next = NULL;
	return (popped_lst);
}
