/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_return.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:15:13 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 17:31:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	perror_ret_int(char *err, int retno)
{
	perror(err);
	return (retno);
}

void	*perror_ret_nullptr(char *err)
{
	perror(err);
	return (NULL);
}

int	ambiguous_error(char *filename)
{
	ft_dprintf(STDERR_FILENO, ERRMSG_AMBIGUOUS, filename);
	return (FAILURE);
}

int	openfile_error(char *filename, char *strerror)
{
	ft_dprintf(STDERR_FILENO, ERRMSG_FILEOPEN, filename, strerror);
	return (FAILURE);
}
