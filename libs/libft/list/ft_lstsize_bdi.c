/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bdi.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 10:04:59 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 10:05:40 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/libft.h"

size_t	ft_lstsize_bdi(t_list_bdi *lst)
{
	size_t	size;

	size = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}
