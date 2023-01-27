/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_same_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 21:47:03 by takira            #+#    #+#             */
/*   Updated: 2023/01/27 21:47:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"

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
