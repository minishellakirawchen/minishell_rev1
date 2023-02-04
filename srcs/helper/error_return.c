/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_return.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:15:13 by takira            #+#    #+#             */
/*   Updated: 2023/02/04 11:27:32 by wchen            ###   ########.fr       */
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

