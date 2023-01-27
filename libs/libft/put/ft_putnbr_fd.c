/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:36:57 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:20:53 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_put.h"

static void	recursion_put_nbr(int num, int fd)
{
	if (num == 0)
		return ;
	recursion_put_nbr(num / 10, fd);
	ft_putchar_fd(num % 10 + '0', fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	num;

	if (n >= 0)
		num = n;
	else
	{
		num = -n;
		ft_putchar_fd('-', fd);
	}
	recursion_put_nbr(num / 10, fd);
	ft_putchar_fd(num % 10 + '0', fd);
}
