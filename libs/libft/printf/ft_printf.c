/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 18:18:38 by takira            #+#    #+#             */
/*   Updated: 2023/01/16 12:38:03 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

static t_printf_info	*init_info(int fd)
{
	t_printf_info	*info;

	errno = 0;
	info = (t_printf_info *)malloc(sizeof(t_printf_info));
	if (!info)
	{
		perror("malloc");
		return (NULL);
	}
	info->fmt_idx = 0;
	info->fd = fd;
	return (info);
}

int	ft_dprintf(int fd, const char *fmt, ...)
{
	t_printf_info	*info;
	ssize_t			sum_print_bytes;
	va_list			arg;

	if (!fmt || fd < 0 || OPEN_MAX < fd)
		return (-1);
	info = init_info(fd);
	if (!info)
		return (-1);
	va_start(arg, fmt);
	sum_print_bytes = printf_controller(info, fmt, &arg);
	va_end(arg);
	free(info);
	return (sum_print_bytes);
}

int	ft_printf(const char *fmt, ...)
{
	t_printf_info	*info;
	ssize_t			sum_print_bytes;
	va_list			arg;

	if (!fmt)
		return (-1);
	info = init_info(STDOUT_FILENO);
	if (!info)
		return (-1);
	va_start(arg, fmt);
	sum_print_bytes = printf_controller(info, fmt, &arg);
	va_end(arg);
	free(info);
	return (sum_print_bytes);
}
