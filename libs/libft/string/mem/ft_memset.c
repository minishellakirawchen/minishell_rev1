/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:36:19 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:26:46 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char		*str;
	const unsigned char	chr = (const unsigned char)c;
	size_t				i;

	str = (unsigned char *)s;
	i = 0;
	while (i < n)
		str[i++] = chr;
	return (s);
}

void	*ft_memset_ns(void *s, int c, size_t n)
{
	unsigned char		*str;
	const unsigned char	chr = (const unsigned char)c;
	size_t				i;

	if (!s)
		return (NULL);
	str = (unsigned char *)s;
	i = 0;
	while (i < n)
		str[i++] = chr;
	return (s);
}
