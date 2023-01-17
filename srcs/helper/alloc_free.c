/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:00:09 by takira            #+#    #+#             */
/*   Updated: 2023/01/17 18:54:40 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_1d_alloc(void *alloc)
{
	free(alloc);
	return (NULL);
}

void	*free_info(t_info *info)
{
	if (!info)
		return (NULL);
	ft_lstclear(&info->envlist, free_env_elem);
	free(info);
	return (NULL);
}

void	*perror_ret_nullptr(char *err)
{
	perror(err);
	return (NULL);
}

void	free_env_elem(void *content)
{
	t_env_elem	*elem;

	if (!content)
		return ;
	elem = content;
	elem->key = free_1d_alloc(elem->key);
	elem->value = free_1d_alloc(elem->value);
	free_1d_alloc(elem);
}