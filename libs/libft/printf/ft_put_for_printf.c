/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_for_printf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 10:15:31 by takira            #+#    #+#             */
/*   Updated: 2023/01/16 11:15:44 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

ssize_t	ft_putchar_for_printf(char c, int fd)
{
	return (write(fd, &c, 1));
}

ssize_t	ft_putstr_for_printf(char *s, int fd)
{
	ssize_t	put_bytes;

	put_bytes = 0;
	if (s == NULL)
		return (0);
	while (*s)
	{
		errno = 0;
		put_bytes += ft_putchar_for_printf(*s++, fd);
		if (errno != 0)
		{
			perror("write");
			return (-1);
		}
		if (put_bytes > INT_MAX)
			return (-1);
	}
	return (put_bytes);
}
