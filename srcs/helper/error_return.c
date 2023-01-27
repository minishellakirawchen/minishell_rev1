/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_return.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:15:13 by takira            #+#    #+#             */
/*   Updated: 2023/01/22 20:16:20 by takira           ###   ########.fr       */
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

