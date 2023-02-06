/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 19:40:14 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 11:47:28 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

// wil = *a*
//        ^

// str = abc
//       ^


// a*
// *a*
// *a
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
		return (NULL);
	ft_strlcat_ns(concat_str, *dst, dstlen + 1);
	ft_strlcat_ns(concat_str, src, dstlen + srclen + 1);
	free(*dst);
	*dst = NULL;
	return (concat_str);
}

static void	free_dp_array(int **dp, size_t row_size)
{
	size_t	i;

	if (!dp || !*dp)
		return ;
	i = 0;
	while (i < row_size)
	{
		free(dp[i]);
		i++;
	}
	free(dp);
}

int **get_dp_table(size_t row, size_t col)
{
	int		**dp;
	size_t	idx;

	dp = (int **)ft_calloc(sizeof(int *), row + 1);
	if (!dp)
	{
		perror("malloc");
		return (NULL);
	}

	idx = 0;
	while (idx < row)
	{
		dp[idx] = (int *)ft_calloc(sizeof(int), col + 1);
		if (!dp[idx])
		{
			perror("malloc");
			return (NULL);
		}
		idx++;
	}
	return (dp);
}

void	print_dp_array(int **array, const char *wild, const char *str)
{
	size_t	row;
	size_t	col;
	size_t	wild_len = ft_strlen_ns(wild);
	size_t	str_len = ft_strlen_ns(str);

	if (!array)
		return ;
	/*
	row = 0;
	while (row < wild_len + 2)
	{
		col = 0;
		while (col < str_len + 2)
		{
			if ((row == 0 && col == 0) || (row == 1 && col == 0) || (row == 0 && col == 1))
				printf("- ");
			else if (row == 0)
				printf("%c ", str[col - 2]);
			else if (col == 0)
				printf("%c ", wild[row - 2]);
			else
				printf("%d ", array[row - 1][col - 1]);
			col++;
		}
		row++;
		printf("\n");
	}
	printf("\n");
	*/
	row = 0;
	while (row < wild_len + 2)
	{
		col = 0;
		while (col < str_len + 2)
		{
			if ((row == 0 && col == 0) || (row == 1 && col == 0) || (row == 0 && col == 1))
				printf("- ");
			else if (row == 0)
				printf("%c ", str[col - 2]);
			else if (col == 0)
				printf("%c ", wild[row - 2]);
			else
				printf("%d ", array[row - 1][col - 1]);
			col++;
		}
		row++;
		printf("\n");
	}
	printf("\n");
}

static int	is_match(const char *wildcard_str, const char *target_str, int **dp, const int *valid_table)
{
	const size_t	len_wildcard = ft_strlen_ns(wildcard_str);
	const size_t	len_target = ft_strlen_ns(target_str);
	size_t			i;
	size_t			j;

	if (!dp || !*dp)
		return (false);
	dp[0][0] = 1;
	i = 1;
	while (i < len_wildcard + 1)
	{
		j = 0;
		while (j < len_target + 1)
		{
			if (valid_table[i - 1] == 1) //wildcard
			{
				dp[i][j] = MAX_SIZE(dp[i - 1][j], dp[i][j]);
				if (j > 0)
					dp[i][j] = MAX_SIZE(dp[i][j - 1], dp[i][j]);
//				printf("i:%zu, j:%zu, j-1:%zu\n", i, j, j-1);
			}
			if (j > 0 && wildcard_str[i - 1] == target_str[j - 1] && valid_table[i - 1] == 0)
				dp[i][j] = MAX_SIZE(dp[i][j], dp[i - 1][j - 1]);
			j++;
		}
		i++;
	}
	print_dp_array(dp, wildcard_str, target_str);
	return (dp[len_wildcard][len_target] > 0);
}

int	is_matches_wildcard_and_target_str(const char *wildcard_str, const char *target_str, const int *valid_table)
{
	const size_t	len_wildcard = ft_strlen_ns(wildcard_str);
	const size_t	len_target = ft_strlen_ns(target_str);
	int				**dp;
	int				ans;

	if (!target_str || !wildcard_str || !valid_table)
		return (false);
	dp = get_dp_table(len_wildcard + 1, len_target + 1);
	if (!dp)
		return (false);
	ans = is_match(wildcard_str, target_str, dp, valid_table);
	free_dp_array(dp, len_wildcard + 1);
	return (ans);
}

void	print_wildcard_valid_list(int *list, char *word)
{
	size_t	idx = 0;

	printf("(");
	while (word[idx])
	{
		printf("%d", list[idx]);
		idx++;
		if (word[idx])
			printf(",");
	}
	printf(")\n");
}

int	test_wildcart(int no, char *input_wild, char *test_str, bool expected, int *valid_list)
{
	bool	ret = is_matches_wildcard_and_target_str(input_wild, test_str, valid_list);
	printf("[TEST %02d] wildcard:%-10s, test_str:%-10s, valid_list:", no, input_wild, test_str);
	print_wildcard_valid_list(valid_list, input_wild);
	printf("\nret:%d, expected:%d, res:%s\n", ret, expected, ret == expected ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m");
	printf("\n\n\n");
	if (ret == expected)
		return (0);
	return (1);
}

int main(void)
{

	int	no = 1;
	int ng = 0;
	int *valid_list = (int *)ft_calloc(sizeof(int), 10);

	valid_list[0] = 0;
	ng += test_wildcart(no++, "*", "a", false, valid_list);

	valid_list[0] = 1;
	ng += test_wildcart(no++, "*", "a", true, valid_list);

	valid_list[0] = 0;
	valid_list[1] = 1;
	valid_list[2] = 0;
	ng += test_wildcart(no++, "a**", "a", false, valid_list);
	ng += test_wildcart(no++, "a**", "aa", false, valid_list);
	ng += test_wildcart(no++, "a**", "aaa", false, valid_list);
	ng += test_wildcart(no++, "a**", "aaaaa*", true, valid_list);

	valid_list[0] = 1;
	valid_list[1] = 1;
	valid_list[2] = 1;
	valid_list[3] = 1;
	valid_list[4] = 0;
	valid_list[5] = 1;
	valid_list[6] = 1;
	valid_list[7] = 1;
	valid_list[8] = 1;
	ng += test_wildcart(no++, "****a****", "a", true, valid_list);

	valid_list[0] = 0;
	valid_list[1] = 0;
	valid_list[2] = 0;
	valid_list[3] = 0;
	valid_list[4] = 0;
	valid_list[5] = 1;
	ng += test_wildcart(no++, "*hoge*", "*hoge", true, valid_list);

	/*
	ng += test_wildcart(no++, "12345*", "12345", true, valid_list);
	ng += test_wildcart(no++, "*", "a", true, valid_list);
	ng += test_wildcart(no++, "**", "a", true, valid_list);
	ng += test_wildcart(no++, "***", "a", true, valid_list);
	ng += test_wildcart(no++, "*a", "a", true, valid_list);
	ng += test_wildcart(no++, "a*", "a", true, valid_list);
	ng += test_wildcart(no++, "*a*", "a", true, valid_list);
	ng += test_wildcart(no++, "****a****", "a", true, valid_list);
	ng += test_wildcart(no++, "i*", "infile", true, valid_list);
	ng += test_wildcart(no++, "n*", "infile", false, valid_list);
	ng += test_wildcart(no++, "*n*", "infile", true, valid_list);
	ng += test_wildcart(no++, "**l", "infile", false, valid_list);
	ng += test_wildcart(no++, "*e*", "infile", true, valid_list);
	ng += test_wildcart(no++, "i*i*e", "infile", true, valid_list);
	ng += test_wildcart(no++, "*i*n*f*i*l*e*", "infile", true, valid_list);
	ng += test_wildcart(no++, "*1*", "file10", true, valid_list);
	ng += test_wildcart(no++, "*a*", "hello", false, valid_list);
	ng += test_wildcart(no++, "*abcx", "abc___abce___abcex", false, valid_list);
	ng += test_wildcart(no++, "*abd*", "abc___abce_abd__abcex", true, valid_list);
	ng += test_wildcart(no++, "*****aba*****ab", "baaababbbbbbbab", true, valid_list);
*/
//	ng += test_wildcart(no++, "A*A*A*A**ABC*****ABX*AB*AB*B*B*B*ABC", "AxAxAxAxABCxAxAAABCxABCxABX1xxABX2xABX3xACxxxxxABXxABxABxxxxBxBxBxABC", true);
//	ng += test_wildcart(no++, "A*A*A*A**ABC*****ABX*AB*AB*B*B*B*ABC", "AxAAABCxABCxABX1xxABX2xxxABX3xACxxxABXxABxxABxxxBxBxBxABC", true);
//	ng += test_wildcart(no++, "A*A*A*A**ABC*****ABX*AB*AB*B*B*B*ABC", "AxAxAxAxABCxAxAAABCxABCxABX1xxABX2xABX3xACxxxxxABXxABxABxxxxBxBxABCD", false);
//	ng += test_wildcart(no++, "A*A*A*A**ABC*****ABX*AB*AB*B*B*B*ABC", "AxAAABCxAbCxABX1xxABX2xxxABX3xACxxxABXxABxxABxxxBxBxBxABC", false);


	printf("############################################\n");
	printf(" TEST RESULT :: %s\n", ng == 0 ? "\x1b[32mALL AC !!!\x1b[0m" : "\x1b[31mWA :X\x1b[0m");
	printf("############################################\n\n");


//	printf("\n");
//	printf("get_wildcard_str:[%s]\n", get_expand_wildcard("*"));


	system("leaks -q a.out");
	return (0);
}
//"A * A * A * A **    ABC ***** ABX  * AB *      AB * B * B * B *   ABC"
//"AxAAABCx            AbCxABX1xxABX2xxxABX3xACxxxABXxABxxABxxxBxBxBxABC"




//"A*A*A*A**ABC*****ABX*                   AB* AB*  B*  B*B*ABC"
//"AxAA  ^  ABCxAbCxABX1xxABX2xxxABX3xACxxxABXxABxxABxxxBxBxBxABC"




















