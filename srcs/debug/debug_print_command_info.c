/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print_command_info.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 20:31:18 by takira            #+#    #+#             */
/*   Updated: 2023/02/12 14:11:02 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_subshell(t_command_info *command_info, bool *is_first_print)
{
	if (command_info && command_info->subshell_token_list)
	{
		if (!*is_first_print)
			ft_dprintf(STDERR_FILENO, "             ");
		ft_dprintf(STDERR_FILENO, "%-18s:", "subshell(token)");
		debug_print_tokens(command_info->subshell_token_list, NULL);
		*is_first_print = false;
	}
}

static void	print_commands(t_command_info *command_info, bool *is_first_print)
{
	if (command_info && command_info->pipeline_token_list)
	{
		if (!*is_first_print)
			ft_dprintf(STDERR_FILENO, "             ");
		ft_dprintf(STDERR_FILENO, "%-18s:", "commands(token)");
		debug_print_tokens(command_info->pipeline_token_list, NULL);
		*is_first_print = false;
	}
	if (command_info && command_info->commands)
	{
		if (!*is_first_print)
			ft_dprintf(STDERR_FILENO, "             ");
		ft_dprintf(STDERR_FILENO, "%-18s:", "commands(char**)");
		debug_print_2d_arr(command_info->commands, NULL);
		*is_first_print = false;
	}
}

static void	print_redirect(t_command_info *command_info, bool *is_first_print)
{
	if (command_info && command_info->redirect_list)
	{
		if (!*is_first_print)
			ft_dprintf(STDERR_FILENO, "             ");
		ft_dprintf(STDERR_FILENO, "%-18s:", "redirect(token)");
		debug_print_redirect_info(command_info->redirect_list, NULL);
		*is_first_print = false;
	}
}

void	debug_print_command_info(t_command_info *command_info)
{
	bool	is_first_print;

	is_first_print = true;
	ft_dprintf(STDERR_FILENO, "  [pipeline] ");
	print_subshell(command_info, &is_first_print);
	print_commands(command_info, &is_first_print);
	print_redirect(command_info, &is_first_print);
}
