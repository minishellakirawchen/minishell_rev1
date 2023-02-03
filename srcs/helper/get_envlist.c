/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envlist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:13:19 by takira            #+#    #+#             */
/*   Updated: 2023/02/03 22:34:33 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* prototype declaration*/
static t_env_elem	*get_env_elem(const char *environ_i, bool *is_shlvl);
static int			add_envelem_from_environ(const char *environ_elem, bool *is_shlvl, t_list **envlist);
static int			get_key_value_from_environ(const char *environ_elem, char **key, char **value);

/* function */
t_list	*get_envlist(void)
{
	extern char	**environ;
	t_list		*env_list_head;
	size_t		idx;
	bool		is_shlvl_exist;

	idx = 0;
	if (!environ)
		return (NULL);
	is_shlvl_exist = false;
	env_list_head = NULL;
	errno = 0;
	while (environ[idx])
	{
		if (add_envelem_from_environ(environ[idx], &is_shlvl_exist, &env_list_head) == FAILURE)
		{
			ft_lstclear(&env_list_head, free_env_elem);
			return (FAILURE);
		}
		idx++;
	}
	if (!is_shlvl_exist && add_initial_shlvl(&env_list_head) == FAILURE)
	{
		ft_lstclear(&env_list_head, free_env_elem);
		return (NULL);
	}
	return (env_list_head);
}

static int	add_envelem_from_environ(const char *environ_elem, bool *is_shlvl, t_list **envlist)
{
	t_env_elem	*env_elem;
	t_list		*new_node;

	if (!environ_elem || !envlist)
		return (FAILURE);

	env_elem = get_env_elem(environ_elem, is_shlvl);
	if (!env_elem)
		return (FAILURE);
	new_node = ft_lstnew(env_elem);
	if (!new_node)
	{
		free(env_elem);
		return (FAILURE);
	}
	ft_lstadd_back(envlist, new_node);
	return (SUCCESS);
}

t_env_elem	*create_new_envelem(char *key, char *value, int not_print)
{
	t_env_elem	*elem;

	elem = (t_env_elem *)malloc(sizeof(t_env_elem));
	if (!elem)
		return (perror_ret_nullptr("malloc"));
	elem->key = key;
	elem->value = value;
	elem->not_print = not_print;
	return (elem);
}

// '='idx
//  v  v len
// 0123
// A=B
//   ^ len-idx-1

static int	get_key_value_from_environ(const char *environ_elem, char **key, char **value)
{
	size_t			idx;
	const size_t	len = ft_strlen_ns(environ_elem);

	if (!environ_elem || !key || !value)
		return (FAILURE);
	idx = 0;
	while (environ_elem[idx] && environ_elem[idx] != '=')
		idx++;
	*key = ft_substr(environ_elem, 0, idx);
	if (!*key)
		return (perror_and_return_int("malloc", FAILURE));
	if (environ_elem[idx] == '\0')//key only
	{
		*value = NULL;
		return (SUCCESS);
	}
	*value = ft_substr(environ_elem, idx + 1, len - idx - 1);
	if (!*value)
	{
		*key = free_1d_alloc(*key);
		return (perror_and_return_int("malloc", FAILURE));
	}
	return (SUCCESS);
}

static t_env_elem	*get_env_elem(const char *environ_i, bool *is_shlvl)
{
	t_env_elem		*elem;
	char			*key;
	char			*value;
	int				not_print;

	if (!environ_i)
		return (NULL);
	key = NULL;
	value = NULL;
	if (get_key_value_from_environ(environ_i, &key, &value) == FAILURE)
		return (NULL);
	not_print = 0;
	if (!value)
		not_print = 1;
	elem = create_new_envelem(key, value, not_print);
	if (!elem)
	{
		free(key);
		free(value);
		return (NULL);
	}
	if (!is_same_str(elem->key, "SHLVL"))
		return (elem);
	*is_shlvl = true;
	if (update_shlvl(&elem->value) == FAILURE)
	{
		free_env_elem(elem);
		return (perror_ret_nullptr("malloc"));
	}
	return (elem);
}
