/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:33:05 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 15:53:54 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_std.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ret_ptr;

	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}
	if (count > SIZE_MAX / size)
		return (NULL);
	ret_ptr = malloc(count * size);
	if (ret_ptr == NULL)
		return (NULL);
	ft_bzero(ret_ptr, count * size);
	return (ret_ptr);
}
