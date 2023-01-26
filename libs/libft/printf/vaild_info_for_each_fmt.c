/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_flg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:42:16 by takira            #+#    #+#             */
/*   Updated: 2023/01/16 10:36:31 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static int	is_valid_strs(t_printf_info info, char type)
{
	if (info.width_siz > INT_MAX || info.perc_siz > INT_MAX)
		return (FAIL);
	if (type == 'c')
	{
		if (info.flag_sign || info.flag_space || info.flag_zero_pad)
			return (FAIL);
		if (info.prec_dot && !info.prec_dot_only)
			return (FAIL);
		return (PASS);
	}
	if (type == 's')
	{
		if (info.flag_sign || info.flag_space || \
		info.flag_zero_pad || info.flag_prefix)
			return (FAIL);
		return (PASS);
	}
	return (FAIL);
}

static int	is_valid_num(t_printf_info *info, char type)
{
	if (info->width_siz > INT_MAX || info->perc_siz > INT_MAX)
		return (FAIL);
	if (info->flag_zero_pad && info->prec_dot)
		info->flag_zero_pad = false;
	if (type == 'd')
	{
		if ((info->flag_sign && info->flag_space) || info->flag_prefix)
			return (FAIL);
		if (info->flag_zero_pad && info->flag_left)
			return (FAIL);
		return (PASS);
	}
	if (type == 'u')
	{
		if (info->flag_sign || info->flag_space || info->flag_prefix)
			return (FAIL);
		return (PASS);
	}
	return (FAIL);
}

static int	is_valid_hex(t_printf_info *info, char type)
{
	if (info->width_siz > INT_MAX || info->perc_siz > INT_MAX)
		return (FAIL);
	if (type == 'x')
	{
		if (info->flag_zero_pad && info->prec_dot)
			info->flag_zero_pad = false;
		if (info->flag_sign || info->flag_space)
			return (FAIL);
		return (PASS);
	}
	if (type == 'p')
	{
		info->is_pointer = true;
		if (info->flag_sign || info->flag_space || \
		info->flag_zero_pad || info->flag_prefix)
			return (FAIL);
		if (info->prec_dot && !info->prec_dot_only)
			return (FAIL);
		info->flag_prefix = true;
		return (PASS);
	}
	return (FAIL);
}

int	valid_info4fmt(char c, t_printf_info *info)
{
	if (c == 'c')
		return (is_valid_strs(*info, 'c'));
	if (c == 's')
		return (is_valid_strs(*info, 's'));
	if (c == 'd' || c == 'i')
		return (is_valid_num(info, 'd'));
	if (c == 'u')
		return (is_valid_num(info, 'u'));
	if (c == 'x' || c == 'X')
		return (is_valid_hex(info, 'x'));
	if (c == 'p')
		return (is_valid_hex(info, 'p'));
	if (c == '%')
		return (PASS);
	return (FAIL);
}
