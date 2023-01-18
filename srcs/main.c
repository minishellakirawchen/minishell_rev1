/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 13:31:13 by takira            #+#    #+#             */
/*   Updated: 2023/01/18 20:45:43 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_key_value(void *content)
{
	t_env_elem	*elem;

	elem = content;
	ft_printf("%s=%s\n", elem->key, elem->value);
}

static int	init_info(t_info *info)
{
	info->envlist_head = get_envlist();
	if (!info->envlist_head)
	{
		info->envlist_head = free_1d_alloc(info->envlist_head);
		return (FAILURE);
	}
	info->tokenlist_head = NULL;
//	ft_dprintf(STDERR_FILENO, "## envlist_head ##\n");
//	ft_lstiter(info->envlist_head, print_key_value);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	int		exit_status;
	t_info	info;

	if (argc != 1 || !argv)
	{
		ft_dprintf(STDERR_FILENO,
				   "[Error]Too many argument."
				       "       Input following:$> ./minishell");
		return (EXIT_FAILURE);
	}
	// init info
	info = (t_info){ 0 };

	init_info(&info);

	// prompt loop
	exit_status = prompt_loop(&info);

	// free param
	free_info(&info);
//	system("leaks -q minishell");
	return (exit_status);
}

/*
__attribute__((destructor))
static void	destructor(void)
{
	system("leaks -q minishell");
}
*/