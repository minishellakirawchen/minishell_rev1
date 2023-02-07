/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:47:59 by takira            #+#    #+#             */
/*   Updated: 2023/02/07 22:49:27 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

// 0123456789
// :hoge:huga ->"", "hoge","huga"
// ^
// :: -> cnt++;


// [::] -> cnt++;
// [],[],[]

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

static char	**get_splitted_words(char const *src, char delim, char ***array, size_t *arr_idx)
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
		else.s
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


static size_t	count_array_size(const char **array)
{
	size_t	idx;

	idx = 0;
	while (array[idx])
		idx++;
	return (idx);
}

static int	test_split(int no, const char *str, const char delim, const char *exp_arr[])
{
	int		test_wa = 0;

	char	**split = ft_split_empty(str, delim);

	size_t	split_size = count_array_size((const char **)split);
	size_t	str_len = ft_strlen_ns(str);
	size_t	exp_size = count_array_size(exp_arr);

	char	*ans_size = (split_size == exp_size) ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m";
	if (split_size != exp_size)
		test_wa = 1;

	printf("[%02d]\n str  :[%s], len:%zu, delim:[%c] -> expsize:%zu, split_size:%zu ;size:%s, \n", no, str, str_len, delim, exp_size, split_size, ans_size);
	if (!split)
		printf(" split: NULL\n");
	else
	{
		size_t	i = 0;
		printf(" split:[");
		while (split[i])
		{
			if (i != 0)
				printf(",");
			printf("%s", split[i++]);
		}
		printf("]\n");

		printf(" expec:[");
		i = 0;
		while (exp_arr[i])
		{
			if (i != 0)
				printf(",");
			printf("%s", exp_arr[i++]);
		}
		printf("]\n");

		i = 0;
		size_t	ng = 0;
		while (exp_arr[i])
		{
			if (ft_strlen_ns(exp_arr[i]) != ft_strlen_ns(split[i]))
				ng++;
			else if (ft_strncmp_ns(exp_arr[i], split[i], ft_strlen_ns(exp_arr[i])) != 0)
				ng++;
			i++;
		}
		printf("  >> result:%s\n\n", (ng == 0) ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m");
		if (ng)
			test_wa = 1;
	}
	return (test_wa);
}

int main(void)
{
	int 	no = 1;
	int 	ng = 0;

	printf("\n\n\n     ##### SPLIT TEST START #####\n\n");

	const char *ans1[] = {"user/bin", "/bin", "/bin/hoge", NULL};
	ng += test_split(no++, "user/bin:/bin:/bin/hoge", ':', ans1);

	const char *ans2[] = {"", "user/bin", "/bin", "/bin/hoge","", NULL};
	ng += test_split(no++, ":user/bin:/bin:/bin/hoge:", ':', ans2);

	const char *ans3[] = {"", NULL};
	ng += test_split(no++, "", ':', ans3);

	const char *ans4[] = {"", "", NULL};
	ng += test_split(no++, ":", ':', ans4);

	const char *ans5[] = {"", "","", "", NULL};
	ng += test_split(no++, ":::", ':', ans5);

	const char *ans6[] = {"","user/bin", "/bin","/bin/hoge", "","","", NULL};
	ng += test_split(no++, ":user/bin:/bin:/bin/hoge:::", ':', ans6);


	printf("RESULT: %s\n", ng == 0 ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m");
	printf("     ##### SPLIT TEST END #####\n\n\n");
}



































