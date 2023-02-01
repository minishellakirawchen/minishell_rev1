/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 19:40:14 by takira            #+#    #+#             */
/*   Updated: 2023/02/01 19:24:32 by takira           ###   ########.fr       */
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

/* return true if wildcard_str matches str */
bool	is_matches_wildcard_and_str(char *wildcard_str, char *target_str)
{
	size_t	i;
	size_t	j;
	size_t	len_wildcard = ft_strlen_ns(wildcard_str);
	size_t	len_target = ft_strlen_ns(target_str);
	int		**dp;
	bool	ans;

//	if (!target_str || !wildcard_str || len_wildcard == 0 || len_target == 0)
	if (!target_str || !wildcard_str)
		return (false);

	dp = get_dp_table(len_wildcard + 1, len_target + 1);
	if (!dp)
		return (false);

	//dp[i][j] : wildcardのi文字目, targetのj文字目まで見て文字列が成立すれば1, 成立しなければ0
	dp[0][0] = 1;

	i = 0;
	while (i < len_wildcard + 1)
	{
		j = 0;
		while (j < len_target + 1)
		{
			if (wildcard_str[i - 1] == '*')
				dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
			if ((j > 0) && (wildcard_str[i - 1] == target_str[j - 1]))
				dp[i][j] = max(dp[i][j], dp[i - 1][j - 1]);
			j++;
		}
		i++;

	}
	printf("result\n");
	print_dp_array(dp, wildcard_str, target_str);

	ans = dp[len_wildcard][len_target] > 0;
	free_dp_array(dp, len_wildcard + 1);
	return (ans);
}


char	*get_expand_wildcard(char *wildcard_str)
{
	DIR 			*open_dir;
	struct dirent	*read_dir;
	char			*expanded_str;
	char			*pwd_path;

	if (!wildcard_str)
		return (NULL);
	expanded_str = ft_strdup("");
	if (!expanded_str)
		return (NULL);

	pwd_path = getcwd(NULL, 0);
	if (!pwd_path)
		return (NULL);
	open_dir = opendir(pwd_path);
	if (!open_dir)
		return (NULL);
	read_dir = readdir(open_dir);
	while (read_dir)
	{
		if (is_same_str(read_dir->d_name, ".") || is_same_str(read_dir->d_name, ".."))
		{
			read_dir = readdir(open_dir);
			continue ;
		}
		if (is_matches_wildcard_and_str(wildcard_str, read_dir->d_name))
		{
			if (ft_strlen_ns(expanded_str) > 0)
				expanded_str = concat_dst_to_src(&expanded_str, " ");
			expanded_str = concat_dst_to_src(&expanded_str, read_dir->d_name);
		}
		read_dir = readdir(open_dir);
	}
	closedir(open_dir);

	free(pwd_path);
//	free(wildcard_str);
	return (expanded_str);
}


int	test_wildcart(int no, char *input_wild, char *test_str, bool expected)
{
	bool	ret = is_matches_wildcard_and_str(input_wild, test_str);
	printf("\n[TEST %02d] wildcard:%-10s, test_str:%-10s, ret:%d, expected:%d, res:%s\n", no, input_wild, test_str, ret, expected, ret == expected ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m");
	printf("\n\n");
	if (ret == expected)
		return (0);
	return (1);
}

int main(void)
{

	int	no = 1;
	int ng = 0;

	ng += test_wildcart(no++, "*", "a", true);
	ng += test_wildcart(no++, "*a", "a", true);
	ng += test_wildcart(no++, "*a*", "a", true);
	ng += test_wildcart(no++, "**a", "a", true);
	ng += test_wildcart(no++, "i*i*e", "infile", true);
	ng += test_wildcart(no++, "A*A*A", "AA", false);

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
	ng += test_wildcart(no++, "*abcx", "abc___abce___abcex", false);
	ng += test_wildcart(no++, "*abd*", "abc___abce_abd__abcex", true);
	ng += test_wildcart(no++, "*****aba*****ab", "baaababbbbbbbab", true);

	ng += test_wildcart(no++, "A*A*A*A**ABC*****ABX*AB*AB*B*B*B*ABC", "AxAxAxAxABCxAxAAABCxABCxABX1xxABX2xABX3xACxxxxxABXxABxABxxxxBxBxBxABC", true);
	ng += test_wildcart(no++, "A*A*A*A**ABC*****ABX*AB*AB*B*B*B*ABC", "AxAAABCxABCxABX1xxABX2xxxABX3xACxxxABXxABxxABxxxBxBxBxABC", true);
	ng += test_wildcart(no++, "A*A*A*A**ABC*****ABX*AB*AB*B*B*B*ABC", "AxAxAxAxABCxAxAAABCxABCxABX1xxABX2xABX3xACxxxxxABXxABxABxxxxBxBxABCD", false);
	ng += test_wildcart(no++, "A*A*A*A**ABC*****ABX*AB*AB*B*B*B*ABC", "AxAAABCxAbCxABX1xxABX2xxxABX3xACxxxABXxABxxABxxxBxBxBxABC", false);


	printf("############################################\n");
	printf(" TEST RESULT :: %s\n", ng == 0 ? "\x1b[32mALL AC !!!\x1b[0m" : "\x1b[31mWA :X\x1b[0m");
	printf("############################################\n\n");


	printf("\n");
	printf("get_wildcard_str:[%s]\n", get_expand_wildcard("*"));


	system("leaks -q a.out");
	return (0);
}
//"A * A * A * A **    ABC ***** ABX  * AB *      AB * B * B * B *   ABC"
//"AxAAABCx            AbCxABX1xxABX2xxxABX3xACxxxABXxABxxABxxxBxBxBxABC"




//"A*A*A*A**ABC*****ABX*                   AB* AB*  B*  B*B*ABC"
//"AxAA  ^  ABCxAbCxABX1xxABX2xxxABX3xACxxxABXxABxxABxxxBxBxBxABC"




















