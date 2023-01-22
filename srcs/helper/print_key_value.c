/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_key_value.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 23:47:15 by wchen             #+#    #+#             */
/*   Updated: 2023/01/22 21:02:36 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	print_key_value(void *content)
{
	t_env_elem	*elem;

	elem = content;
	ft_printf("%s = %s\n", elem->key, elem->value);
}
