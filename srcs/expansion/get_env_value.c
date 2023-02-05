/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 10:02:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 12:11:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*get_name_str(const char *str_start_with_dollar)
{
	const char	*src = str_start_with_dollar;
	char		*name_str;
	size_t		idx;

	idx = 0;
	if (!src || src[idx] != CHR_DOLLAR)
		return (NULL);
	idx++;
	if (!src[idx])
		return (NULL);
	while (src[idx] && (src[idx] == '_' || ft_isalnum(src[idx])))
		idx++;
	name_str = ft_substr(src, 1, idx - 1);
	if (!name_str)
		return (perror_ret_nullptr("malloc"));
	return (name_str);
}

/* return value if key exists. otherwise return '' */
/* if value is null or has not print flag, return '' */
char	*get_env_value(const char *search_key, t_list *env_list_head)
{
	const size_t	search_key_len = ft_strlen_ns(search_key);
	t_env_elem		*elem;

	if (search_key_len == 0)
		return ("");
	while (env_list_head)
	{
		elem = env_list_head->content;
		if ((ft_strlen_ns(elem->key) == search_key_len) \
		&& (ft_strncmp_ns(elem->key, search_key, search_key_len) == 0))
		{
			if (!elem->value)
				return ("");
			return (elem->value);
		}
		env_list_head = env_list_head->next;
	}
	return ("");
}
