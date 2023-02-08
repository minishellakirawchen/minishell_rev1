/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_array_size.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:21:05 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 13:23:02 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"

size_t	get_arr_size(char **array)
{
	size_t	size;

	size = 0;
	while (array && array[size])
		size++;
	return (size);
}
