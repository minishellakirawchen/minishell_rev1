/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envlist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:13:19 by takira            #+#    #+#             */
/*   Updated: 2023/02/03 21:53:52 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* prototype declaration*/
static t_env_elem	*get_env_elem(const char *environ_i, bool *is_shlvl);
static int	add_envelem_from_environ(const char *environ_elem, bool *is_shlvl, t_list **envlist);


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


// '='idx
//  v  v len
// 0123
// A=B
//   ^ len-idx-1
static t_env_elem	*get_env_elem(const char *environ_i, bool *is_shlvl)
{
	t_env_elem		*elem;
	size_t			idx;
	const size_t	len = ft_strlen_ns(environ_i);

	if (!environ_i)
		return (NULL);
	idx = 0;
	while (environ_i[idx] && environ_i[idx] != '=')
		idx++;
	if (environ_i[idx] == '\0')
		return (NULL);
	errno = 0;
	elem = (t_env_elem *)malloc(sizeof(t_env_elem));
	if (!elem)
		return (perror_ret_nullptr("malloc"));
	elem->key = ft_substr(environ_i, 0, idx);
	elem->value = ft_substr(environ_i, idx + 1, len - idx - 1);
	elem->not_print = 0;
	if (!elem->key || !elem->value)
	{
		free_env_elem(elem);
		elem = NULL;
		return (perror_ret_nullptr("malloc"));
	}
	if (is_same_str(elem->key, "SHLVL"))
	{
		*is_shlvl = true;
		if (update_shlvl(&elem->value) == FAILURE)
		{
			free_env_elem(elem);
			return (perror_ret_nullptr("malloc"));
		}
	}
	return (elem);
}
