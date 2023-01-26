/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bdi.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 09:49:29 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 09:57:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/libft.h"

void	ft_lstiter_bdi(t_list_bdi *lst, void (*f)(void *))
{
	if (lst == NULL || f == NULL)
		return ;
	while (lst != NULL)
	{
		f(lst->content);
		lst = lst->next;
	}
}
