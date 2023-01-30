/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 19:40:14 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 22:51:36 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	test_wildcart(int testno, char *input, char **teststrs, char **expected)
{

}


int main(void)
{
	char *input[10] = {
		"*",
		"**",
		"***********",
		"*a",
		"*a*",
		"**a",
		"*a*",
		"a*",
		NULL
	};
	char *teststrs[10][10] = {
			{"a", "b", "c", NULL},
			{"a", "b", "c", NULL},
			{NULL},
	};
	char *expected[10][10] = {


			{NULL},
	};

	int	idx = 0;

	while (input[idx])
	{
		test_wildcart(idx, input[idx], teststrs[idx], expected[idx]);
		idx++;
	}



	return (0);
}