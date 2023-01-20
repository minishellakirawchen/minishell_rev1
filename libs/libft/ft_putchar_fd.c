/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:36:29 by takira            #+#    #+#             */
/*   Updated: 2023/01/20 16:46:13 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <errno.h>
# include <stdio.h>

ssize_t	ft_putchar_fd(char c, int fd)
{
	ssize_t	ret;

	errno = 0;
	ret = write(fd, &c, sizeof(char));
	if (ret < 0)
		perror("write");
	return (ret);
}
