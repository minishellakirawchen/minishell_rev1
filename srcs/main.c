/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 13:31:13 by takira            #+#    #+#             */
/*   Updated: 2023/02/04 09:35:08 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_info	*init_info(void)
{
	t_info	*info;

	info = (t_info *)malloc(sizeof(t_info));
	if (!info)
		return (perror_ret_nullptr("malloc"));
	info->envlist_head = get_envlist();
	if (!info->envlist_head)
	{
		info = free_1d_alloc(info);
		return (FAILURE);
	}
	info->tokenlist_head = NULL;
	info->execlist_head = NULL;
	return (info);
}

static void	display_ascii_art(void)
{
	return ;
	ft_printf("           _       _      _          _ _ \n" \
	" _ __ ___ (_)_ __ (_)___ | |__   ___| | |\n" \
	"| '_ ` _ \\| | '_ \\| / __|| '_ \\ / _ \\ | |\n" \
	"| | | | | | | | | | \\__ \\| | | |  __/ | |\n" \
	"|_| |_| |_|_|_| |_|_|___/|_| |_|\\___|_|_|\n" \
	"                        By: takira, wchen\n");
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
	// init info
	info = init_info();
	if (!info)
		return (FAILURE);
	display_ascii_art();
	// prompt loop
	exit_status = prompt_loop(info);
	// free param
	free_info(&info);
//	system("leaks -q minishell");
	return (exit_status);
}
