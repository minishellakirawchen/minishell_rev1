/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:36:29 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:21:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_put.h"

ssize_t	ft_putchar_fd(char c, int fd)
{
	ssize_t	ret;

	errno = 0;
	ret = write(fd, &c, sizeof(char));
	if (ret < 0)
		perror("write");
	return (ret);
}
