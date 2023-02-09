/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:38:13 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 23:28:52 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*joined_str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	joined_str = (char *)ft_calloc(sizeof(char), s1_len + s2_len + 1);
	if (joined_str == NULL)
		return (NULL);
	ft_memcpy(&joined_str[0], s1, s1_len);
	ft_memcpy(&joined_str[s1_len], s2, s2_len);
	return (joined_str);
}
