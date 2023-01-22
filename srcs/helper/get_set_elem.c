/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set_elem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 19:04:28 by wchen             #+#    #+#             */
/*   Updated: 2023/01/22 20:40:01 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static char	**get_value(t_env_elem *elem, char *key)
{
	printf("elem_key:%s\n", elem->key);
	printf("key:%s\n", key);
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
		printf("test\n");
		value = get_value(env_node->content, key);
		if (value != NULL)
			return (value);
		env_node = env_node->next;
	}
	return (value);
}

int	set_elem(t_info *info, char *key, char *value)
{
	t_list		*new_env_node;
	t_env_elem	*new_elem;

	new_elem = malloc(sizeof(t_env_elem));
	if (!new_elem)
		perror_and_return_int("malloc", EXIT_FAILURE);
	new_elem -> key = key;
	new_elem -> value = value;
	new_env_node = ft_lstnew(new_elem);
	if (!new_env_node)
		perror_and_return_int("malloc", EXIT_FAILURE);
	if (!info->envlist_head)
		info->envlist_head = new_env_node;
	else
		ft_lstadd_back(&info->envlist_head, new_env_node);
	return (EXIT_SUCCESS);
}
