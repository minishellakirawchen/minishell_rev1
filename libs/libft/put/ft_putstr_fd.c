/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:37:09 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:21:00 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_put.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (s == NULL)
		s = "(null)";
	while (*s != '\0')
		ft_putchar_fd(*s++, fd);
}