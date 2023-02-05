/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_expanded_str.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 23:19:06 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 10:40:03 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/* free dest and assign nullptr in this function to overwrite dst */
/* ex) dst = concat_dst_to_sec(&dst, src) */
char	*concat_dst_to_src(char **dst, char *src)
{
	size_t	dstlen;
	size_t	srclen;
	char	*concat_str;

	if (!dst)
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

static int	skip_to_expandable_str(char **src, size_t *idx, char **expanded_str)
{
	size_t	skip;
	char	*skip_str;

	if (!src || !expanded_str)
		return (FAILURE);
	skip = 0;
	while ((*src)[*idx + skip] \
	&& !is_expandable_str_with_dollar(&(*src)[*idx + skip]))
		skip++;
	if (skip > 0)
	{
		skip_str = ft_substr(*src, *idx, skip);
		if (!skip_str)
			return (perror_ret_int("malloc", FAILURE));
		*expanded_str = concat_dst_to_src(expanded_str, skip_str);
		if (!*expanded_str)
			return (FAILURE);
		free_1d_alloc(skip_str);
		*idx += skip;
	}
	return (SUCCESS);
}

static int	expand_exit_status(char **expanded_str, int exit_status)
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

static int	expand_exit_status_or_env_var(\
char **src, t_info *info, size_t *idx, char **expanded_str)
{
	char	*key;
	char	*value;

	if (!src || !info || !idx || !expanded_str)
		return (FAILURE);
	if (is_expandable_exit_status(&(*src)[*idx]))
	{
		if (expand_exit_status(expanded_str, info->exit_status) == FAILURE)
			return (FAILURE);
		*idx += 2;
	}
	else
	{
		key = get_name_str(&(*src)[*idx]);
		value = get_env_value(key, info->envlist_head);
		if (!key | !value)
			return (perror_ret_int("malloc", FAILURE));
		*expanded_str = concat_dst_to_src(expanded_str, value);
		if (!*expanded_str)
			return (FAILURE);
		*idx += ft_strlen_ns(key) + 1;
		key = free_1d_alloc(key);
	}
	return (SUCCESS);
}

int	expand_var_in_str(char **src, t_info *info)
{
	size_t	idx;
	char	*expanded_str;

	if (!src || !*src || !info)
		return (FAILURE);
	expanded_str = ft_strdup("");
	idx = 0;
	while ((*src)[idx])
	{
		if (skip_to_expandable_str(src, &idx, &expanded_str) == FAILURE)
			return (FAILURE);
		if (!(*src)[idx])
			break ;
		if (expand_exit_status_or_env_var(\
		src, info, &idx, &expanded_str) == FAILURE)
			return (FAILURE);
	}
	free(*src);
	*src = expanded_str;
	return (SUCCESS);
}
