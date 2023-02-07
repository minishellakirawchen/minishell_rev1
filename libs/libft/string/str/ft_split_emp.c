/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_emp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:29:44 by takira            #+#    #+#             */
/*   Updated: 2023/02/07 22:56:50 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"
#include "../../../include/ft_gnl.h"

static char	**free_array_ret_nullptr(char **arr)
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

static char	**get_splitted_words(\
char const *src, char delim, char ***array, size_t *arr_idx)
{
	size_t	len;
	size_t	head_idx;

	head_idx = 0;
	*arr_idx = 0;
	while (src[head_idx])
	{
		len = 0;
		while (src[head_idx + len] && src[head_idx + len] != delim)
			len++;
		if (src[head_idx + len] && src[head_idx + len] == delim)
			len++;
		if (src[head_idx + len - 1] == delim)
			(*array)[*arr_idx] = ft_substr(src, head_idx, len - 1);
		else
			(*array)[*arr_idx] = ft_substr(src, head_idx, len);
		if (!(*array)[*arr_idx])
			return (free_array_ret_nullptr((*array)));
		head_idx += len;
		*arr_idx += 1;
	}
	return (*array);
}

/* split_emp is not delete empty string between delimiter. such as */
/*  src="hoge::huga:", delim=':'       */
/*  ret={"hoge", "", "huga", "", NULL} */
char	**ft_split_empty(char const *src, char delim)
{
	size_t	num_of_words;
	char	**splitted_words;
	size_t	arr_idx;

	if (src == NULL)
		return (NULL);
	num_of_words = cnt_chr_in_str(delim, src) + 1;
	splitted_words = (char **)ft_calloc(num_of_words + 1, sizeof(char *));
	if (splitted_words == NULL)
		return (NULL);
	splitted_words = get_splitted_words(src, delim, &splitted_words, &arr_idx);
	if (!splitted_words)
		return (NULL);
	while (arr_idx < num_of_words)
	{
		splitted_words[arr_idx] = ft_strdup("");
		if (!splitted_words[arr_idx])
			return (free_array_ret_nullptr(splitted_words));
		arr_idx++;
	}
	return (splitted_words);
}
