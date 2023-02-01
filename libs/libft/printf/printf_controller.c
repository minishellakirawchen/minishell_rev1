/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:52:04 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:18:55 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

static void	init_info_for_fmt(t_printf_info *info)
{
	info->flag_left = false;
	info->flag_sign = false;
	info->flag_space = false;
	info->flag_prefix = false;
	info->flag_zero_pad = false;
	info->prec_dot = false;
	info->prec_dot_only = false;
	info->is_pointer = false;
	info->width_siz = 0;
	info->perc_siz = 0;
	info->num_base = 10;
	info->num_capitals = 0;
	info->num_padlen = 0;
	info->num_preclen = 0;
	info->num_head_chr = "";
}

static ssize_t	pass_to_printfunc(const char c, t_printf_info info, va_list *p)
{
	if (c == 'c')
		return (print_c(va_arg(*p, int), info));
	if (c == 's')
		return (print_s(va_arg(*p, char *), info));
	if (c == 'p')
		return (print_hex(va_arg(*p, uintptr_t), &info, false));
	if (c == 'd' || c == 'i')
		return (print_signed(va_arg(*p, int), &info));
	if (c == 'u')
		return (print_unsigned(va_arg(*p, unsigned int), info));
	if (c == 'x')
		return (print_hex(va_arg(*p, unsigned int), &info, false));
	if (c == 'X')
		return (print_hex(va_arg(*p, unsigned int), &info, true));
	if (c == '%')
		return (print_percent(info));
	return (-1);
}

static ssize_t	print_fmt(char *fmt, t_printf_info *info, va_list *p)
{
	init_info_for_fmt(info);
	get_flag((char *)fmt, info);
	if (errno != 0)
		return (-1);
	if (get_width((char *) fmt, info, p) == FAILURE)
		return (-1);
	if (get_prec((char *) fmt, info, p) == FAILURE)
		return (-1);
	if (valid_info4fmt(fmt[info->fmt_idx], info) == FAILURE)
		return (-1);
	return (pass_to_printfunc(fmt[info->fmt_idx], *info, p));
}

int	printf_controller(t_printf_info *info, const char *fmt, va_list *ap)
{
	ssize_t	print_bytes;
	ssize_t	sum_bytes;

	if (!fmt || !*fmt || !info)
		return (-1);
	sum_bytes = 0;
	while (fmt[info->fmt_idx] && errno == 0 && sum_bytes >= 0)
	{
		errno = 0;
		while (fmt[info->fmt_idx] != '%' && fmt[info->fmt_idx] && errno == 0)
			sum_bytes += ft_putchar_for_printf(fmt[info->fmt_idx++], info->fd);
		if (!fmt[info->fmt_idx])
			break ;
		info->fmt_idx++;
		print_bytes = print_fmt((char *)fmt, info, ap);
		sum_bytes += print_bytes;
		if (errno != 0 || print_bytes == -1 || sum_bytes > INT_MAX)
			sum_bytes = -1;
		info->fmt_idx++;
	}
	return ((int)sum_bytes);
}
