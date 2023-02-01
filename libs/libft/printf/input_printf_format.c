/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_printf_format.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 22:06:48 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:18:22 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

void	get_flag(char *fmt, t_printf_info *info)
{
	const char		*flags = "-+ #0";

	while (fmt[info->fmt_idx] && \
	ft_strchr_printf(flags, fmt[info->fmt_idx]) != NULL)
	{
		if (fmt[info->fmt_idx] == '-')
			info->flag_left = true;
		if (fmt[info->fmt_idx] == '+')
			info->flag_sign = true;
		if (fmt[info->fmt_idx] == ' ')
			info->flag_space = true;
		if (fmt[info->fmt_idx] == '#')
			info->flag_prefix = true;
		if (fmt[info->fmt_idx] == '0')
			info->flag_zero_pad = true;
		info->fmt_idx++;
	}
}

int	get_width(char *fmt, t_printf_info *info, va_list *p)
{
	int	input_arg;

	if (fmt[info->fmt_idx] == '*' || ft_isdigit_pf(fmt[info->fmt_idx]))
	{
		if (fmt[info->fmt_idx] == '*')
		{
			input_arg = va_arg(*p, int);
			if (input_arg >= 0)
				info->width_siz = input_arg;
			else
			{
				info->width_siz = -input_arg;
				info->flag_left = true;
				info->flag_zero_pad = false;
			}
			info->fmt_idx++;
			if (info->flag_zero_pad && ft_isdigit_pf(fmt[info->fmt_idx]))
				return (FAILURE);
		}
		while (ft_isdigit_pf(fmt[info->fmt_idx]))
			info->width_siz = info->width_siz * 10 + fmt[info->fmt_idx++] - '0';
		if (fmt[info->fmt_idx] == '*')
			return (FAILURE);
	}
	return (SUCCESS);
}

static void	update_prec_params(t_printf_info *info)
{
	if (info->perc_siz < 0)
	{
		info->prec_dot = false;
		info->perc_siz = 0;
	}
	if (info->perc_siz)
		info->prec_dot_only = false;
}

int	get_prec(char *fmt, t_printf_info *info, va_list *p)
{
	if (fmt[info->fmt_idx] == '.')
	{
		info->prec_dot = true;
		info->prec_dot_only = true;
		info->fmt_idx += 1;
		if (fmt[info->fmt_idx] == '*')
		{
			info->perc_siz = va_arg(*p, int);
			info->prec_dot_only = false;
			info->fmt_idx += 1;
			if (ft_isdigit_pf(fmt[info->fmt_idx]))
				return (FAILURE);
		}
		while (ft_isdigit_pf(fmt[info->fmt_idx]))
			info->perc_siz = info->perc_siz * 10 + fmt[info->fmt_idx++] - '0';
		update_prec_params(info);
		if (fmt[info->fmt_idx] == '*')
			return (FAILURE);
	}
	return (SUCCESS);
}
