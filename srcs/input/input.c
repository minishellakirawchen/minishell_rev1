/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:55 by takira            #+#    #+#             */
/*   Updated: 2023/01/19 13:55:34 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

void	clear_input(t_info *info)
{
	if (!info)
		return ;
	ft_lstclear(&info->tokenlist_head, free_token_elem);
	info->tokenlist_head = NULL;
}

int	prompt_loop(t_info *info)
{
	int		exit_status;
	char	*input_line;
	bool	is_continue;


	if (!info)
		return (FAILURE);
	exit_status = EXIT_SUCCESS;
	while (true)
	{
		is_continue = true;
		input_line = readline(PROMPT);
		if (!input_line)
		{
			ft_dprintf(STDERR_FILENO, "^D Pressed. Exit minishell\n");
			break ;
		}
		if (is_same_str(input_line, ""))
		{
			input_line = free_1d_alloc(input_line);
//			ft_dprintf(STDERR_FILENO, "NL only, Not add to history\n");
			continue ;
		}

		ft_dprintf(STDERR_FILENO, "#%-15s:[%s]\n", "input", input_line);
		// tokenize
		if (tokenize_input_line(info, input_line) == FAILURE)
			return (FAILURE);//free

		// input validation (Mandatory/Bonus)
		if (validate_token_list(info->tokenlist_head) == FAILURE)
			is_continue = false;//add_history & free(line)

		if (is_continue && arrange_token_list(info) == FAILURE)
			return (FAILURE);

		// parsing (Mandatory/Bonus)
		// expansion
		// command_execution
		add_history(input_line);
		input_line = free_1d_alloc(input_line);
		clear_input(info);
	}
	return (exit_status);
}
