/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 19:40:14 by takira            #+#    #+#             */
/*   Updated: 2023/01/31 18:45:04 by takira           ###   ########.fr       */
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

int **get_dp_table(size_t row, size_t col)
{
	int		**dp;
	size_t	idx;

	dp = (int **)calloc(sizeof(int *), row + 1);
	if (!dp)
	{
		perror("malloc");
		return (NULL);
	}

	idx = 0;
	while (idx < row)
	{
		dp[idx] = (int *)calloc(sizeof(int), col + 1);
		if (!dp[idx])
		{
			perror("malloc");
			return (NULL);
		}
		idx++;
	}
	return (dp);
}

void	print_dp_array(int **array, char *wild, char *str)
{
	size_t	row;
	size_t	col;
	size_t	wild_len = ft_strlen_ns(wild);
	size_t	str_len = ft_strlen_ns(str);

	if (!array)
		return ;

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

/* return true if wildcard matches str */
bool	is_matches_wildcard_and_str(char *input_wild, char *test_str)
{
	size_t	i;
	size_t	j;
	size_t	len_wild = ft_strlen_ns(input_wild);
	size_t	len_str = ft_strlen_ns(test_str);
	int		**dp;

	if (!test_str || !input_wild || len_wild == 0 || len_str == 0)
		return (false);

	// *, **, ..., ****...
	if (len_wild == cnt_chr_in_str('*', test_str))
		return (true);

	dp = get_dp_table(len_wild + 1, len_str + 1);
	if (!dp)
		return (false);

	//dp[i][j] : wildのi-1文字目, strのj-1文字目まで見て文字列が成立すれば1, そうでなければ0
	dp[0][0] = 1;
//	printf("init\n");
//	print_dp_array(dp, input_wild, test_str);

//	printf("while\n");
	i = 1;
	while (i < len_wild + 1)
	{
		j = 1;
		while (j < len_str + 1)
		{
//			printf(" i:%zu, j:%zu\n", i, j);
			if (input_wild[i - 1] == '*')
			{
				dp[i][j] = max(dp[i - 1][j - 1], dp[i][j - 1]);
				dp[i][j] = max(dp[i - 1][j], dp[i][j]);
			}
			else if (input_wild[i - 1] == test_str[j - 1])
				dp[i][j] = max(dp[i - 1][j - 1], dp[i - 1][j]);
//			printf("(i,j,c,dp)=(%zu, %zu, %c%c, %d)\n", i, j, input_wild[i-1], test_str[j-1], dp[i][j]);
			j++;
		}
		i++;
//		print_dp_array(dp, input_wild, test_str);

	}
	printf("result\n");
	print_dp_array(dp, input_wild, test_str);
//	printf("dp:%d, i:%zu, j:%zu\n", dp[len_wild][len_str], len_wild, len_str);
	if (dp[len_wild][len_str] > 0)
		return (true);
	return (false);
	//TODO:free
}

int	test_wildcart(int no, char *input_wild, char *test_str, bool expected)
{
	bool	ret = is_matches_wildcard_and_str(input_wild, test_str);
	printf("[TEST %02d] wildcard:%-10s, test_str:%-10s, ret:%d, expected:%d, res:%s\n", no, input_wild, test_str, ret, expected, ret == expected ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m\n\n");
	if (ret == expected)
		return (0);
	return (1);
}

int main(void)
{

	int	no = 1;
	int ng = 0;

	ng += test_wildcart(no++, "12345*", "12345", true);
	ng += test_wildcart(no++, "*", "a", true);
	ng += test_wildcart(no++, "**", "a", true);
	ng += test_wildcart(no++, "***", "a", true);
	ng += test_wildcart(no++, "*a", "a", true);
	ng += test_wildcart(no++, "a*", "a", true);
	ng += test_wildcart(no++, "*a*", "a", true);
	ng += test_wildcart(no++, "****a****", "a", true);
	ng += test_wildcart(no++, "i*", "infile", true);
	ng += test_wildcart(no++, "n*", "infile", false);
	ng += test_wildcart(no++, "*n*", "infile", true);
	ng += test_wildcart(no++, "**l", "infile", false);
	ng += test_wildcart(no++, "*e*", "infile", true);
	ng += test_wildcart(no++, "i*i*e", "infile", true);
	ng += test_wildcart(no++, "*i*n*f*i*l*e*", "infile", true);
	ng += test_wildcart(no++, "*1*", "file10", true);
	ng += test_wildcart(no++, "*a*", "hello", false);

	printf("TEST RESULT :: %s\n", ng == 0 ? "\x1b[32mALL AC !!!\x1b[0m" : "\x1b[31mWA :X\x1b[0m");

	return (0);
}