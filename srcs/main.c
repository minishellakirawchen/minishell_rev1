/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 13:31:13 by takira            #+#    #+#             */
/*   Updated: 2023/01/27 22:48:25 by takira           ###   ########.fr       */
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
	// prompt loop
	exit_status = prompt_loop(info);

	// free param
	free_info(&info);
	system("leaks -q minishell");
	return (exit_status);
}

/*
__attribute__((destructor))
static void	destructor(void)
{
	system("leaks -q minishell");
}
*/