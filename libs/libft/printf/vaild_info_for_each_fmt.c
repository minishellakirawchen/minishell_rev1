/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_flg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:42:16 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:19:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

static int	is_valid_strs(t_printf_info info, char type)
{
	if (info.width_siz > INT_MAX || info.perc_siz > INT_MAX)
		return (FAILURE);
	if (type == 'c')
	{
		if (info.flag_sign || info.flag_space || info.flag_zero_pad)
			return (FAILURE);
		if (info.prec_dot && !info.prec_dot_only)
			return (FAILURE);
		return (SUCCESS);
	}
	if (type == 's')
	{
		if (info.flag_sign || info.flag_space || \
		info.flag_zero_pad || info.flag_prefix)
			return (FAILURE);
		return (SUCCESS);
	}
	return (FAILURE);
}

static int	is_valid_num(t_printf_info *info, char type)
{
	if (info->width_siz > INT_MAX || info->perc_siz > INT_MAX)
		return (FAILURE);
	if (info->flag_zero_pad && info->prec_dot)
		info->flag_zero_pad = false;
	if (type == 'd')
	{
		if ((info->flag_sign && info->flag_space) || info->flag_prefix)
			return (FAILURE);
		if (info->flag_zero_pad && info->flag_left)
			return (FAILURE);
		return (SUCCESS);
	}
	if (type == 'u')
	{
		if (info->flag_sign || info->flag_space || info->flag_prefix)
			return (FAILURE);
		return (SUCCESS);
	}
	return (FAILURE);
}

static int	is_valid_hex(t_printf_info *info, char type)
{
	if (info->width_siz > INT_MAX || info->perc_siz > INT_MAX)
		return (FAILURE);
	if (type == 'x')
	{
		if (info->flag_zero_pad && info->prec_dot)
			info->flag_zero_pad = false;
		if (info->flag_sign || info->flag_space)
			return (FAILURE);
		return (SUCCESS);
	}
	if (type == 'p')
	{
		info->is_pointer = true;
		if (info->flag_sign || info->flag_space || \
		info->flag_zero_pad || info->flag_prefix)
			return (FAILURE);
		if (info->prec_dot && !info->prec_dot_only)
			return (FAILURE);
		info->flag_prefix = true;
		return (SUCCESS);
	}
	return (FAILURE);
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
		return (SUCCESS);
	return (FAILURE);
}
