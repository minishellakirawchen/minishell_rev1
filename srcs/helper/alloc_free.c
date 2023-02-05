/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:00:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 15:11:44 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	**free_2d_alloc(void **alloc)
{
	size_t	i;

	if (!alloc)
		return (NULL);
	i = 0;
	while (alloc[i])
	{
		alloc[i] = free_1d_alloc(alloc[i]);
		i++;
	}
	free(alloc);
	return (NULL);
}

void	*free_1d_alloc(void *alloc)
{
	free(alloc);
	return (NULL);
}

void	*free_allocs_ret_nullptr(void *alloc1, void *alloc2)
{
	free_1d_alloc(alloc1);
	free_1d_alloc(alloc2);
	return (NULL);
}

void	*free_info(t_info **info)
{
	if (!info || !*info)
		return (NULL);
	ft_lstclear(&(*info)->envlist_head, free_env_elem);
	(*info)->envlist_head = NULL;
	clear_input_info(info);
	free(*info);
	*info = NULL;
	return (NULL);
}
