/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_math.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:34:59 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 15:42:28 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_math.h"

int	max_int(int a, int b)
{
	if (a >= b)
		return (a);
	return (b);
}

int	min_int(int a, int b)
{
	if (a <= b)
		return (a);
	return (b);
}

size_t	max_size(size_t a, size_t b)
{
	if (a >= b)
		return (a);
	return (b);
}

size_t	min_size(size_t a, size_t b)
{
	if (a <= b)
		return (a);
	return (b);
}
