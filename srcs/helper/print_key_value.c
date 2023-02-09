/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_key_value.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 23:47:15 by wchen             #+#    #+#             */
/*   Updated: 2023/01/30 22:50:36 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_key_value(void *content)
{
	t_env_elem	*elem;

	elem = content;
	if (!elem)
		return ;
	if (elem->value != NULL && elem->not_print == 0)
		ft_printf("%s=%s\n", elem->key, elem->value);
}

void	print_export_key_value(void *content)
{
	t_env_elem	*elem;

	elem = content;
	if (!elem)
		return ;
	if (elem->value != NULL && elem->not_print == 0)
		ft_printf("declare -x %s=\"%s\"\n", elem->key, elem->value);
	else if (elem->not_print == 0)
		ft_printf("declare -x %s\n", elem->key);
}
