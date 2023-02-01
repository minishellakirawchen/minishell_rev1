/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_expanded_str.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 23:19:06 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 22:56:32 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

// expandable_src is string start with $name or $?
// $

// "" removal
char	*get_expanded_str(char *src, t_info *info)
{
	size_t	idx;
	size_t	skip;
	char	*expanded_str;
	char	*key;
	char	*value;
	char	*skip_str;

	if (!src || !info)
		return (FAILURE);
	expanded_str = ft_strdup("");
	idx = 0;
	while (src[idx])
	{
		// $? or $nameまでidx++
		skip = 0;
//		printf("idx:%zu, src:%s\n", idx, &src[idx + skip]);
		while (src[idx + skip] && !is_expandable_str_with_dollar(&src[idx + skip]))
			skip++;
		if (skip)
		{
			// idx++した分をexpanded_strへ結合
			skip_str = ft_substr(src, idx, skip);
			expanded_str = concat_dst_to_src(&expanded_str, skip_str);
			if (!skip_str || !expanded_str)
				return (perror_ret_nullptr("malloc"));
			skip_str = free_1d_alloc(skip_str);
			idx += skip;
		}
		if (!src[idx])
			break ;
		// $? or $name のvalueをexpanded_strへ結合子、$? or $name分idx++
		if (is_expandable_exit_status(&src[idx]))
		{
			if (expand_exit_status(&expanded_str, info->exit_status) == FAILURE)
				return (NULL);
			idx += 2; // $?
		}
		else
		{
//			printf("idx:%zu, src[idx]:%s\n", idx, &src[idx]);
			key = get_name_str(&src[idx]);
			value = get_env_value(key, info->envlist_head);
//			printf("key:%s, value:%s\n", key, value);
			if (!key | !value)
				return (perror_ret_nullptr("malloc"));
			expanded_str = concat_dst_to_src(&expanded_str, value);
			idx += ft_strlen_ns(key); // $key
			key = free_1d_alloc(key);
		}
		idx++;
	}
	free_1d_alloc(src);
	return (expanded_str);
}

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
//	printf("name :[%s]\n", name_str);
	return (name_str);
}

char *get_env_value(const char *search_key, t_list *env_list_head)
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
			return (elem->value);
		env_list_head = env_list_head->next;
	}
	return ("");
}

/* free dest and assign nullptr in this function to overwrite dst */
/* ex) dst = concat_dst_to_sec(&dst, src) */
char	*concat_dst_to_src(char **dst, char *src)
{
	size_t	dstlen;
	size_t	srclen;
	char	*concat_str;

	if (!dst || !src)
		return (NULL);
	dstlen = ft_strlen_ns(*dst);
	srclen = ft_strlen_ns(src);
	concat_str = (char *)ft_calloc(sizeof(char), dstlen + srclen + 1);
	if (!concat_str)
		return (perror_ret_nullptr("malloc"));
	ft_strlcat_ns(concat_str, *dst, dstlen + 1);
	ft_strlcat_ns(concat_str, src, dstlen + srclen + 1);
	free(*dst);
	*dst = NULL;
	return (concat_str);
}

int	expand_exit_status(char **expanded_str, int exit_status)
{
	char	*status_itoa;

	if (!expanded_str)
		return (FAILURE);
	status_itoa = ft_itoa(exit_status);
	if (!status_itoa)
		return (perror_ret_int("malloc", FAILURE));
	*expanded_str = concat_dst_to_src(expanded_str, status_itoa);
	free(status_itoa);
	if (!*expanded_str)
		return (FAILURE);
	return (SUCCESS);
}
