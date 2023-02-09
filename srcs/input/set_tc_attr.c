/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tc_attr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 12:26:22 by wchen             #+#    #+#             */
/*   Updated: 2023/02/09 12:23:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_tc_attr_out_execute(void)
{
	struct termios	tty;
	int				fd;

	fd = open(ttyname(STDIN_FILENO), O_RDWR);
	if (fd < 0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (tcgetattr(fd, &tty) < 0)
	{
		perror("tcgetattr");
		return (1);
	}
	tty.c_lflag &= ~ECHOCTL;
	if (tcsetattr(fd, TCSANOW, &tty) < 0)
	{
		perror("tcsetattr");
		return (1);
	}
	close(fd);
	return (0);
}

int	set_tc_attr_in_execute(void)
{
	struct termios	tty;
	int				fd;

	fd = open(ttyname(STDIN_FILENO), O_RDWR);
	if (fd < 0)
	{
		return (EXIT_FAILURE);
	}
	if (tcgetattr(fd, &tty) < 0)
	{
		perror("tcgetattr");
		return (1);
	}
	tty.c_lflag |= ECHOCTL;
	if (tcsetattr(fd, TCSANOW, &tty) < 0)
	{
		perror("tcsetattr");
		return (1);
	}
	close(fd);
	return (0);
}
