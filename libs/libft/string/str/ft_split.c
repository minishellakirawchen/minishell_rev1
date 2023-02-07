/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:37:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/07 21:32:11 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"

static size_t	count_words(char const *src, char delim)
{
	size_t	i;
	size_t	cnt;

	i = 0;
	cnt = 0;
	if (delim == '\0')
		return (1);
	while (src[i] != '\0')
	{
		if (src[i] != delim)
		{
			cnt++;
			while (src[i] != '\0' && src[i] != delim)
				i++;
		}
		while (src[i] != '\0' && src[i] == delim)
			i++;
	}
	return (cnt);
}

static void	get_split_idx(const char *src, char delim, size_t *h, size_t *t)
{
	while (src[*h] != '\0' && src[*h] == delim)
	{
		*h += 1;
		*t += 1;
	}
	while (src[*t] != '\0' && src[*t] != delim)
		*t += 1;
}

static char	**free_ret(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

static char	**create_split_arr(char **ret_arr, const char *src, char delim)
{
	size_t	i;
	size_t	head_idx;
	size_t	tail_idx;

	i = 0;
	head_idx = 0;
	tail_idx = 0;
	while (src[head_idx] != '\0')
	{
		get_split_idx(src, delim, &head_idx, &tail_idx);
		if (head_idx >= tail_idx)
			break ;
		ret_arr[i] = ft_substr(src, head_idx, tail_idx - head_idx);
		if (ret_arr[i] == NULL)
			return (free_ret(ret_arr));
		head_idx = tail_idx;
		i++;
	}
	ret_arr[i] = NULL;
	return (ret_arr);
}

char	**ft_split(char const *s, char c)
{
	size_t	num_of_words;
	char	**ret_arr;

	if (s == NULL)
		return (NULL);
	num_of_words = count_words(s, c);
	ret_arr = (char **)ft_calloc(num_of_words + 1, sizeof(char *));
	if (ret_arr == NULL)
		return (NULL);
	return (create_split_arr(ret_arr, s, c));
}
