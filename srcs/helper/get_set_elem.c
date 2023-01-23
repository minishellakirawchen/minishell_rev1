/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set_elem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 19:04:28 by wchen             #+#    #+#             */
/*   Updated: 2023/01/23 21:14:41 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_value(t_env_elem *elem, char *key)
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
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	new_elem -> key = key;
	new_elem -> value = value;
	new_env_node = ft_lstnew(new_elem);
	if (!new_env_node)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	if (!info->envlist_head)
		info->envlist_head = new_env_node;
	else
		ft_lstadd_back(&info->envlist_head, new_env_node);
	return (EXIT_SUCCESS);
}

int	append_env(t_info *info, char *key, char *value)
{
	char	**elem_value;
	ssize_t	key_len;
	char 	*elem_key;
	char	*temp_ptr;

	key_len = ft_strlen(key);
	elem_key = malloc(sizeof(char) * (key_len));
	if (!elem_key)
		return(perror_and_return_int("malloc", EXIT_FAILURE));
	elem_key = ft_memmove(elem_key, key, key_len - 1);
	free (key);
	elem_value = get_elem(info, elem_key);
	if (elem_value == NULL)
		set_elem(info, elem_key, value);
	else
	{
		temp_ptr = *elem_value;
		*elem_value = ft_strjoin(*elem_value, value);
		if(!elem_value)
			return(perror_and_return_int("malloc", EXIT_FAILURE));
		free (elem_key);
		free (value);
		free (temp_ptr);
	}
	return (EXIT_SUCCESS);
}

int add_env(t_info *info, char *key, char *value)
{
	int		exit_status;
	char	**elem_value;
	char	*temp_ptr;

	exit_status = EXIT_SUCCESS;
	elem_value = get_elem(info, key);
	if (elem_value != NULL)
	{
		temp_ptr = *elem_value;
		*elem_value = value;
		free(temp_ptr);
		free (key);
	}
	else
		exit_status = set_elem(info, key, value);
	return(exit_status);
}
