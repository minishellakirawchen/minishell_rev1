/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bdi.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 09:49:21 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 09:57:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/libft.h"

void	ft_lstclear_bdi(t_list_bdi **lst, void (*del)(void *))
{
	t_list_bdi	*next;

	if (lst == NULL)
		return ;
	while (*lst != NULL)
	{
		next = (*lst)->next;
		ft_lstdelone_bdi(lst, del);
		*lst = next;
	}
}
