/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_key_value.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 23:47:15 by wchen             #+#    #+#             */
/*   Updated: 2023/01/29 02:07:27 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_key_value(void *content)
{
	t_env_elem	*elem;

	elem = content;
	if (elem->value != NULL)
		ft_printf("%s = %s\n", elem->key, elem->value);
}

void	print_export_key_value(void *content)
{
	t_env_elem	*elem;

	elem = content;
	if (elem->value != NULL)
		printf("declare -x %s=\"%s\"\n", elem->key, elem->value);
	else
		printf("declare -x %s\n", elem->key);
}
