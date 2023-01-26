/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_chr_in_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:15:10 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:27:47 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ft_string.h"

bool	is_chr_in_str(char chr, const char *str)
{
	if (str && ft_strchr(str, chr) != NULL)
		return (true);
	return (false);
}

bool	is_str1chrs_in_str2(const char *str1, const char *str2)
{
	size_t	idx;

	if (!str1 || !str2)
		return (false);
	idx = 0;
	while (str1[idx])
	{
		if (ft_strchr(str2, str1[idx]))
			return (true);
		idx++;
	}
	return (false);
}
