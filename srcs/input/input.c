/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:55 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 15:51:56 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

void	clear_input_info(t_info **info)
{
	if (!info || !*info)
		return ;
	ft_lstclear_bdi(&(*info)->tokenlist_head, free_token_elem);
	clear_exec_list(&(*info)->execlist_head);
	(*info)->readline_input = free_1d_alloc((*info)->readline_input);
}

// FAILURE: 致命的なエラー(malloc)のためexit
// is_continue=false: syntax error等, minishellはexitせず継続
int	prompt_loop(t_info *info)
{
	int		exit_status;
	bool	is_return_input;

	if (!info)
		return (FAILURE);
	exit_status = EXIT_SUCCESS;
	while (true)
	{
		is_return_input = false;
		/* input */
		info->readline_input = readline(PROMPT);
		if (!info->readline_input)
		{
			// debug
			ft_dprintf(STDERR_FILENO, "[#DEBUG]^D Pressed. Exit minishell\n");
			break ;
		}
		if (is_same_str(info->readline_input, ""))
		{
			info->readline_input = free_1d_alloc(info->readline_input);
			continue ;
		}
		// debug
		ft_dprintf(STDERR_FILENO, "#%-15s:[%s]\n", "input", info->readline_input);

		/* tokenize */
		if (tokenize_input_line(info, info->readline_input) == FAILURE)
		{
			ft_dprintf(STDERR_FILENO, "[#DEBUG]tokenize failure\n");
			return (FAILURE);//free
		}

		/* input validation (Mandatory/Bonus) */
		if (arrange_and_validate_token_list(&info->tokenlist_head) == FAILURE)
			is_return_input = true;//add_history & free(line)

		// debug
//		debug_print_tokens(info->tokenlist_head, "arranged");

		/* parsing (Mandatory/Bonus) */
		if (!is_return_input && parsing_token_list(info) == FAILURE)
		{
			ft_dprintf(STDERR_FILENO, "[#DEBUG]parsing failure\n");
			return (FAILURE);
		}
		/* expansion & command_execution */
		exit_status = execute_execlist(info);

		/* clear input */
		add_history(info->readline_input);
		clear_input_info(&info);
	}
	return (exit_status);
}
