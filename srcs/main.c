/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 13:31:13 by takira            #+#    #+#             */
/*   Updated: 2023/01/17 17:52:16 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_info	*init_info(void)
{
	t_info	*info;

	info = (t_info *)malloc(sizeof(t_info));
	if (!info)
		return (NULL);
	info->exit_status = EXIT_SUCCESS;
	info->envlist = NULL;
	return (info);
}

int	main(int argc, char **argv)
{
	int		exit_status;
	t_info	*info;

	if (argc != 1 || !argv)
	{
		ft_dprintf(STDERR_FILENO,
				   "[Error]Too many argument."
				       "       Input following:$> ./minishell");
		return (EXIT_FAILURE);
	}
	// init param
	info = init_info();
	if (!info)
		return (EXIT_FAILURE);

	// prompt loop
	exit_status = prompt_loop(info);

	// free param
	info = free_info(info);
	return (exit_status);
}
