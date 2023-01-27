/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ptr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 09:24:38 by takira            #+#    #+#             */
/*   Updated: 2023/01/22 09:29:16 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	main(void)
{
	int	*num1;
	int *num2;
	int m = 1;

	num1 = &m;
	num2 = num1;
	printf("num1:%d num2:%d\n", *num1, *num2);

	int k = 2;
	num1 = &k;
	printf("num1:%d num2:%d\n", *num1, *num2);

	num1 = NULL;
	printf("num2:%d\n", *num2);

	//num1:1 num2:1
	//num1:2 num2:1
	//num2:1
	return (0);
}