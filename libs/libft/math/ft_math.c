/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_math.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:34:59 by takira            #+#    #+#             */
/*   Updated: 2023/01/22 13:40:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

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
