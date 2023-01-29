/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set_elem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 19:04:28 by wchen             #+#    #+#             */
/*   Updated: 2023/01/29 23:23:59 by wchen            ###   ########.fr       */
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

int	set_elem(t_info *info, char *key, char *value)
{
	t_list		*new_env_node;
	t_env_elem	*new_elem;

	new_elem = malloc(sizeof(t_env_elem));
	if (!new_elem)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	new_elem->key = key;
	new_elem->value = value;
	new_elem->not_print = 0;
	new_env_node = ft_lstnew(new_elem);
	if (!new_env_node)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	else
		ft_lstadd_back(&info->envlist_head, new_env_node);
	return (EXIT_SUCCESS);
}

int	append_env(t_info *info, char *key, char *value)
{
	char	**elem_value;
	ssize_t	key_len;
	char	*elem_key;
	char	*temp_ptr;

	key_len = ft_strlen(key);
	elem_key = malloc(sizeof(char) * (key_len));
	if (!elem_key)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	elem_key = ft_memmove(elem_key, key, key_len - 1);
	key = free_1d_alloc(key);
	elem_value = get_elem(info, elem_key);
	if (elem_value == NULL)
		set_elem(info, elem_key, value);
	else
	{
		temp_ptr = *elem_value;
		*elem_value = ft_strjoin(*elem_value, value);
		if (!elem_value)
			return (perror_and_return_int("malloc", EXIT_FAILURE));
		elem_key = free_1d_alloc(elem_key);
		value = free_1d_alloc(value);
		temp_ptr = free_1d_alloc(temp_ptr);
	}
	return (EXIT_SUCCESS);
}

int	add_env(t_info *info, char *key, char *value)
{
	int		exit_status;
	char	**elem_value;
	char	*temp_ptr;

	exit_status = EXIT_SUCCESS;
	elem_value = get_elem(info, key);
	if (elem_value != NULL && value != NULL)
	{
		temp_ptr = *elem_value;
		*elem_value = value;
		temp_ptr = free_1d_alloc(temp_ptr);
		key = free_1d_alloc(key);
	}
	else if (elem_value == NULL)
		exit_status = set_elem(info, key, value);
	else
		free(key);
	return (exit_status);
}
