/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 13:59:13 by takira            #+#    #+#             */
/*   Updated: 2023/02/09 14:04:47 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_prompt(int exit_status)
{
	char	*prompt;
	char	*tmp_str;
	char	*exit_status_string;

	exit_status_string = ft_itoa(exit_status);
	errno = 0;
	tmp_str = ft_strjoin("minishell ", exit_status_string);
	if (!tmp_str)
		return (perror_ret_nullptr("malloc"));
	prompt = ft_strjoin(tmp_str, " $> ");
	free(tmp_str);
	free(exit_status_string);
	if (!prompt)
		return (perror_ret_nullptr("malloc"));
	return (prompt);
}
