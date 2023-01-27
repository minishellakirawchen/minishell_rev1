/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:37:48 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:29:17 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"

char	*ft_strdup(const char *s1)
{
	char	*ret_str;
	size_t	len;

	len = ft_strlen(s1);
	ret_str = (char *)ft_calloc(sizeof(char), len + 1);
	if (ret_str == NULL)
		return (NULL);
	ft_memcpy(ret_str, s1, len);
	return (ret_str);
}

char	*ft_strdup_ns(const char *s1)
{
	char	*ret_str;
	size_t	len;

	if (!s1)
	{
		ret_str = (char *)ft_calloc(sizeof(char), 1);
		if (ret_str == NULL)
			return (NULL);
		return (ret_str);
	}
	len = ft_strlen(s1);
	ret_str = (char *)ft_calloc(sizeof(char), len + 1);
	if (ret_str == NULL)
		return (NULL);
	ft_memcpy(ret_str, s1, len);
	return (ret_str);
}
