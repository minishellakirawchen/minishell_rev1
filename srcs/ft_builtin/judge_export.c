/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   judge_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 22:26:30 by wchen             #+#    #+#             */
/*   Updated: 2023/01/23 22:57:59 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_key_type	judge_key(t_export_info *e_info)
{
	ssize_t	i;
	ssize_t	key_len;
	char	*key;

	i = 0;
	key = e_info->key;
	if (ft_isdigit(*key) || *key == '\0')
		return (e_error);
	key_len = ft_strlen(key);
	while (*key)
	{
		if ((i + 1) == key_len && *key == '+' && e_info->key_type != e_novalue)
			return (e_append);
		if (!ft_isalnum(*key) && *key != '_')
			return (e_error);
		i++;
		key++;
	}
	if (e_info->key_type == e_novalue)
		return (e_novalue);
	return (e_add);
}

t_key_type	judge_value(t_export_info *e_info)
{
	char	*c_ptr;

	c_ptr = e_info->value;
	if (!e_info->value)
		return (e_novalue);
	while (*c_ptr != '\0')
	{
		if (!ft_isprint(*c_ptr))
			return (e_error);
		c_ptr++;
	}
	return (e_info->key_type);
}
