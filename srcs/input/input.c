/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:55 by takira            #+#    #+#             */
/*   Updated: 2023/01/19 16:52:33 by takira           ###   ########.fr       */
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

// FAILURE: 致命的なエラー(malloc)のためexit
// is_continue=false: syntax error等, minishellはexitしない
int	prompt_loop(t_info *info)
{
	int		exit_status;
	char	*input_line;
	bool	is_return_input;


	if (!info)
		return (FAILURE);
	exit_status = EXIT_SUCCESS;
	while (true)
	{
		is_return_input = false;
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
		if (arrange_and_validate_token_list(&info->tokenlist_head) == FAILURE)
			is_return_input = true;//add_history & free(line)

		debug_print_token_word(info->tokenlist_head, "arranged");

		if (is_return_input)
			ft_dprintf(STDERR_FILENO, "return input\n");
		// parsing (Mandatory/Bonus)
		// expansion
		// command_execution
		add_history(input_line);
		input_line = free_1d_alloc(input_line);
		clear_input(info);
	}
	return (exit_status);
}
