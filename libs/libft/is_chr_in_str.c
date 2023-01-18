/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_chr_in_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:15:10 by takira            #+#    #+#             */
/*   Updated: 2023/01/18 14:11:30 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

bool is_chr_in_str(const char chr, const char *str)
{
	if (str && ft_strchr(str, chr) != NULL)
		return (true);
	return (false);
}
