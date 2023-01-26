/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bdi.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 09:49:03 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 09:54:22 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/libft.h"

void	ft_lstadd_back_bdi(t_list_bdi **lst, t_list_bdi *new)
{
	t_list_bdi	*last;

	if (lst == NULL || new == NULL)
		return ;
	new->next = NULL;
	if (*lst == NULL)
		*lst = new;
	else
	{
		last = ft_lstlast_bdi(*lst);
		last->next = new;
		new->prev = last;
	}
}
