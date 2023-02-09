/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err_message.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 21:09:55 by wchen             #+#    #+#             */
/*   Updated: 2023/02/09 21:12:54 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_err_message(void)
{
	ft_dprintf(STDERR_FILENO, "cd: error retrieving current directory: ");
	ft_dprintf(STDERR_FILENO, "getcwd: cannot access parent directories: ");
	ft_dprintf(STDERR_FILENO, "directories: No such file or directory \n");
}
