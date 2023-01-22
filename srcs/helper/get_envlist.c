/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envlist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:13:19 by takira            #+#    #+#             */
/*   Updated: 2023/01/20 23:52:22 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* prototype declaration*/
static t_env_elem	*get_env_elem(const char *environ_i);

/* function */
t_list	*get_envlist(void)
{
	extern char	**environ;
	t_list		*env_list_head;
	t_list		*new_node;
	t_env_elem	*env_elem;
	size_t		idx;

	idx = 0;
	if (!environ)
		return (NULL);
//	debug_print_2d_arr(environ, "environ");
	env_list_head = NULL;
	errno = 0;
	while (environ[idx])
	{
		env_elem = get_env_elem(environ[idx]);
		new_node = ft_lstnew(env_elem);
		if (!env_elem || !new_node)
		{
			ft_lstclear(&env_list_head, free_env_elem);
			return (NULL);
		}
		ft_lstadd_back(&env_list_head, new_node);
		idx++;
	}
	return (env_list_head);
}

// '='idx
//  v  v len
// 0123
// A=B
//   ^ len-idx-1

// A= :OK
// A  :NG
static t_env_elem	*get_env_elem(const char *environ_i)
{
	t_env_elem		*elem;
	size_t			idx;
	const size_t	len = ft_strlen_ns(environ_i);

	if (!environ_i)
		return (NULL);//env=NULL
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
	if (!elem->key || !elem->value)
	{
		free_env_elem(elem);
		elem = NULL;
		return (perror_ret_nullptr("malloc"));
	}
	return (elem);
}
