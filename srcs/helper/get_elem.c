/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_elem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 18:30:54 by wchen             #+#    #+#             */
/*   Updated: 2023/02/05 18:31:19 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_value_from_key(t_env_elem *elem, char *key)
{
	if (is_same_str(elem->key, key))
		return (&elem->value);
	return (NULL);
}

char	**get_elem(t_info *info, char *key)
{
	t_list	*env_node;
	char	**value;

	env_node = info->envlist_head;
	value = NULL;
	while (env_node != NULL)
	{
		value = get_value_from_key(env_node->content, key);
		if (value != NULL)
			return (value);
		env_node = env_node->next;
	}
	return (value);
}
