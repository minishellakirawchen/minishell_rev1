/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ret_u.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:15:13 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:18:28 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

static char	*convert_u2base(unsigned long u, int base, int capitals)
{
	size_t			len;
	unsigned long	num;
	char			*ret_str;
	const char		*hex_digit = "0123456789abcdef0123456789ABCDEF";

	num = u;
	len = 0;
	if (u == 0)
		len++;
	while (num > 0)
	{
		num /= base;
		len += 1;
	}
	ret_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!ret_str)
		return (NULL);
	ret_str[len] = '\0';
	while (len--)
	{
		ret_str[len] = hex_digit[u % base + capitals];
		u /= base;
	}
	return (ret_str);
}

static void	set_preclen_and_padlen(unsigned long u, t_printf_info *info)
{
	size_t	len;

	len = 0;
	if (!info->prec_dot_only && u == 0)
		len += 1;
	while (u > 0)
	{
		u /= info->num_base;
		len += 1;
	}
	if ((size_t)info->perc_siz > len)
	{
		info->num_preclen = (size_t)info->perc_siz - len;
		len += info->num_preclen;
	}
	if ((size_t)info->width_siz >= (len + ft_strlen_printf(info->num_head_chr)))
	{
		if (info->num_head_chr)
			len += ft_strlen_printf(info->num_head_chr);
		if (info->flag_zero_pad)
			info->num_preclen = (size_t)info->width_siz - len;
		else
			info->num_padlen = (size_t)info->width_siz - len;
	}
}

ssize_t	print_unsigned(unsigned long u, t_printf_info info)
{
	char		*num_str;
	ssize_t		ret_bytes;

	num_str = convert_u2base(u, info.num_base, info.num_capitals);
	if (!num_str)
		return (-1);
	ret_bytes = 0;
	set_preclen_and_padlen(u, &info);
	if (!info.flag_left && info.num_padlen)
		while (info.num_padlen-- && errno == 0)
			ret_bytes += ft_putchar_for_printf(' ', info.fd);
	if (info.num_head_chr && errno == 0)
		ret_bytes += ft_putstr_for_printf(info.num_head_chr, info.fd);
	if (info.num_preclen)
		while (info.num_preclen-- && errno == 0)
			ret_bytes += ft_putchar_for_printf('0', info.fd);
	if (!(info.prec_dot_only && u == 0) && errno == 0)
		ret_bytes += ft_putstr_for_printf(num_str, info.fd);
	if (info.flag_left && info.num_padlen && errno == 0)
		while (info.num_padlen-- && errno == 0)
			ret_bytes += ft_putchar_for_printf(' ', info.fd);
	free(num_str);
	return (ret_bytes);
}

ssize_t	print_signed(int n, t_printf_info *info)
{
	unsigned int	u;

	if (n >= 0)
	{
		u = n;
		if (info->flag_sign)
			info->num_head_chr = "+";
		if (info->flag_space)
			info->num_head_chr = " ";
	}
	else
	{
		u = -n;
		info->num_head_chr = "-";
	}
	return (print_unsigned(u, *info));
}

ssize_t	print_hex(unsigned long u, t_printf_info *info, bool isupper)
{
	info->num_base = 16;
	if (isupper)
		info->num_capitals = 16;
	if (info->flag_prefix)
	{
		if (info->num_capitals)
			info->num_head_chr = "0X";
		else
			info->num_head_chr = "0x";
	}
	if (!info->is_pointer && u == 0)
		info->num_head_chr = "";
	return (print_unsigned(u, *info));
}
