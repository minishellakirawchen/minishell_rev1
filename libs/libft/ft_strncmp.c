/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:39:29 by takira            #+#    #+#             */
/*   Updated: 2023/01/17 17:11:00 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char	*str1 = (const unsigned char *)s1;
	const unsigned char	*str2 = (const unsigned char *)s2;
	size_t				i;

	if (n == 0)
		return (0);
	i = 0;
	while (str1[i] && str2[i] && i < n - 1)
	{
		if (str1[i] != str2[i])
			break ;
		i++;
	}
	return ((int)(str1[i] - str2[i]));
}

int	ft_strncmp_ns(const char *s1, const char *s2, size_t n)
{
	const unsigned char	*str1 = (const unsigned char *)s1;
	const unsigned char	*str2 = (const unsigned char *)s2;
	size_t				i;

	if (!s1 || !s2)
		return (0);
	if (n == 0)
		return (0);
	i = 0;
	while (str1[i] && str2[i] && i < n - 1)
	{
		if (str1[i] != str2[i])
			break ;
		i++;
	}
	return ((int)(str1[i] - str2[i]));
}

bool	is_same_str(const char *str1, const char *str2)
{
	const size_t	str1_len = ft_strlen_ns(str1);
	const size_t	str2_len = ft_strlen_ns(str2);

	if (str1_len != str2_len)
		return (false);
	if (ft_strncmp_ns(str1, str2, str1_len) == 0)
		return (true);
	return (false);
}
