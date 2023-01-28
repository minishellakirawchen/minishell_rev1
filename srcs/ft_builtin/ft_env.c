/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 21:07:59 by wchen             #+#    #+#             */
/*   Updated: 2023/01/29 00:57:59 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_info *info)
{
	if (!info)
		return (EXIT_FAILURE);
	ft_lstiter(info->envlist_head, print_key_value);
	return (EXIT_SUCCESS);
}

int ft_sort_env(t_info *info)
{
	if (!info)
		return (EXIT_FAILURE);
	ft_lstiter(info->envlist_head, print_export_key_value);
	return (EXIT_SUCCESS);
}
