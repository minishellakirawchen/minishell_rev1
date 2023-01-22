/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 21:07:59 by wchen             #+#    #+#             */
/*   Updated: 2023/01/21 21:01:34 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_env(t_info *info)
{
	// t_list		*envlist_node;
	// char		*key;
	// char		*value;
	// t_env_elem	*elem;

	// envlist_node = info -> envlist_head;
	// while (envlist_node != NULL)
	// {
	// 	elem = envlist_node -> content;
	// 	key = elem -> key;
	// 	value = elem -> value;
	// 	ft_printf("%s=%s\n", key, value);
	// 	envlist_node = envlist_node -> next;
	// }
	if(!info)
		return (EXIT_FAILURE);
	ft_lstiter(info->envlist_head, print_key_value);
	return (0);
}
