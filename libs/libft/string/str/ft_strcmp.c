/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 15:45:14 by takira            #+#    #+#             */
/*   Updated: 2023/02/01 15:46:35 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"

int	ft_strcmp_ns(const char *s1, const char *s2)
{
	const unsigned char	*str1 = (const unsigned char *)s1;
	const unsigned char	*str2 = (const unsigned char *)s2;
	size_t				idx;

	if (!s1 || !s2)
		return (0);
	idx = 0;
	while (str1[idx] && str2[idx])
	{
		if (str1[idx] != str2[idx])
			break ;
		idx++;
	}
	return ((int)(str1[idx] - str2[idx]));
}
