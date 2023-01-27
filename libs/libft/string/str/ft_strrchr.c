/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:41:15 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:28:34 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ret_ptr;
	size_t	i;
	size_t	len;

	ret_ptr = NULL;
	len = ft_strlen(s);
	i = 0;
	while (i <= len)
	{
		if (s[i] == (char)c)
			ret_ptr = (char *)&s[i];
		i++;
	}
	return (ret_ptr);
}
