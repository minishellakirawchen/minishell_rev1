/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:13:26 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:29:44 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"

static char	**free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
	return (NULL);
}

static size_t	get_strs_size(const char *str, char delim, char set)
{
	size_t	size;
	size_t	i;
	bool	is_include_str;

	size = 0;
	i = 0;
	while (str[i])
	{
		is_include_str = false;
		while (str[i] && str[i] == delim)
			i++;
		if (str[i] && str[i] != delim)
			is_include_str = true;
		while (str[i] && str[i] != delim && str[i] != set)
			i++;
		if (str[i] && str[i] == set)
		{
			i++;
			while (str[i] && str[i] != set)
				i++;
			if (str[i] && str[i] == set)
				i++;
		}
		if (str[i] == delim && is_include_str)
		{
			size++;
			is_include_str = false;
		}
	}
	if (is_include_str)
		size++;
	return (size);
}

static size_t	get_substr_size(const char *str, char delim, char set, size_t head)
{
	size_t	size;

	size = 0;
	while (str[head + size])
	{
		while (str[head + size] && str[head + size] != delim && str[head + size] != set)
			size++;
		if (str[head + size] && str[head + size] == set)
		{
			size++;
			while (str[head + size] && str[head + size] != set)
				size++;
			if (str[head + size] && str[head + size] == set)
				size++;
		}
		if (str[head + size] && str[head + size] == delim)
			return (size);
	}
	return (size);
}

// delim=_, set='
// str="aaa___bb'b___'__c"
// ->aaa,bb'b___',c

//head is start to get
//tail is next char in the string to get(= delim or '\0')
static char	**get_split_strs(char ***strs, const char *str, char delim, char set)
{
	size_t	i;
	size_t	head_idx;
	size_t	size;

	i = 0;
	head_idx = 0;
	while (str[head_idx])
	{
		while (str[head_idx] && str[head_idx] == delim)
			head_idx++;
		if (!str[head_idx])
			break ;
		size = get_substr_size(str, delim, set, head_idx);
		(*strs)[i] = ft_substr(str, head_idx, size);
		if (!(*strs)[i])
			return (free_array(*strs));
		i++;
		head_idx += size;
	}
	return (*strs);
}

char	**ft_split_set(const char *str, char delim, char set)
{
	char	**strs;
	size_t	strs_size;

	if (!str || delim == set)
		return (NULL);
	strs_size = 0;
	if (delim)
		strs_size = get_strs_size(str, delim, set);
	strs = (char **)ft_calloc(sizeof(char *), strs_size + 1);
	if (!strs)
		return (NULL);
	get_split_strs(&strs, str, delim, set);
	if (!strs)
		return (NULL);
	return (strs);
}

/*
static int	playground(int no, const char *str, const char delim, const char set, size_t exp_size, char *exp_arr[])
{
	int		test_wa = 0;
	size_t	strs_size = get_strs_size(str, delim, set);
	size_t	str_len = ft_strlen_ns(str);
//	get_strs_size(str, delim, set, &strs_size);

	char	*ans_size = (strs_size == exp_size) ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m";
	if (strs_size != exp_size)
		test_wa = 1;
	char	**split = ft_split_set(str, delim, set);
	printf("[%02d]\n str  :[%s], len:%zu, delim:[%c], set:[%c] -> strs_size:%zu ;size:%s, \n", no, str, str_len, delim, set, strs_size, ans_size);
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

	printf("\n\n     ##### SPLIT TEST START #####\n\n");


	char *ans1[] = {"111","222","33","44","555", NULL};
	ng += playground(no++, "111 222 33 44 555", ' ', '\0', 5, ans1);

	char *ans2[] = {"111 222"," 33"," 44 555", NULL};
	ng += playground(no++, "111 222, 33, 44 555", ',', '\0', 3, ans2);

	char *ans3[] = {"111", "222", "33'44'555", NULL};
	ng += playground(no++, "111 222 33'44'555", ' ', '\'', 3, ans3);

	char *ans4[] = {"'111 222 33 44 555'", NULL};
	ng += playground(no++, "'111 222 33 44 555'", '\0', '\'', 1, ans4);

	char *ans5[] = {"111","222","33","44","555", NULL};
	ng += playground(no++, " 111 222 33 44 555 ", ' ', '\'', 5, ans5);

	char *ans6[] = {"1   1222", "33", "44", "555", NULL};
	ng += playground(no++, "   1   1222 33 44 555  ", ' ', '1', 4, ans6);

	char *ans7[] = {"111", "222", "33", "44", "555**", NULL};
	ng += playground(no++, " 111 222 33 44 555** ", ' ', '*', 5, ans7);

	char *ans8[] = {"*111 222 33 44 555", NULL};
	ng += playground(no++, "*111 222 33 44 555", ' ', '*', 1, ans8);

	char *ans9[] = {"11'1 2'22", "33", "' 44 555  '", NULL};
	ng += playground(no++, "11'1 2'22 33 ' 44 555  '  ", ' ', '\'', 3, ans9);

	char *ans10[] = {"'", NULL};
	ng += playground(no++, "'", ' ', '\'', 1, ans10);

	char *ans11[] = {"", NULL};
	ng += playground(no++, "", ' ', '\'', 0, ans11);

	char *ans12[] = {"echo hello world ", " cat Makefile ", " echo 'playground | playground | playground '", NULL};
	ng += playground(no++, "echo hello world | cat Makefile | echo 'playground | playground | playground '", '|', '\'', 3, ans12);

	char *ans13[] = {"echo hello world ", " cat Makefile ", " echo 'playground | playground | playground ''", NULL};
	ng += playground(no++, "echo hello world | cat Makefile | echo 'playground | playground | playground ''", '|', '\'', 3, ans13);

	char *ans14[] = {"echo hello world ", " cat Makefile ", " echo 'playground | playground | playground ''''", NULL};
	ng += playground(no++, "echo hello world | cat Makefile | echo 'playground | playground | playground ''''", '|', '\'', 3, ans14);

	char *ans15[] = {"echo hello world ", " 'playground | playground ' ", " cat -e out", NULL};
	ng += playground(no++, "echo hello world | 'playground | playground ' | cat -e out", '|', '\'', 3, ans15);

	char *ans16[] = {"echo ", " 'playground || playground| hoge|' ", " cat -e ", " gre", "p", NULL};
	ng += playground(no++, "echo | 'playground || playground| hoge|' ||| cat -e | gre|p", '|', '\'', 5, ans16);


	printf("RESULT: %s\n", ng == 0 ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m");
	printf("     ##### SPLIT TEST END #####\n\n");
}
*/