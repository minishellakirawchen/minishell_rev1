/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:55 by takira            #+#    #+#             */
/*   Updated: 2023/01/23 19:00:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

void	clear_input(t_info **info)
{
	if (!info || !*info)
		return ;
	ft_lstclear(&(*info)->tokenlist_head, free_token_elem);
	clear_exec_list(&(*info)->execlist_head);
	(*info)->tokenlist_head = NULL;
	(*info)->execlist_head = NULL;
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

		/* input */
		input_line = readline(PROMPT);
		if (!input_line)
		{
			ft_dprintf(STDERR_FILENO, "^D Pressed. Exit minishell\n");
			break ;
		}
		if (is_same_str(input_line, ""))
		{
			is_return_input = true;
			input_line = free_1d_alloc(input_line);
//			continue ;
		}

		ft_dprintf(STDERR_FILENO, "#%-15s:[%s]\n", "input", input_line);

		/* tokenize */
		if (!is_return_input && tokenize_input_line(info, input_line) == FAILURE)
		{
			ft_dprintf(STDERR_FILENO, "tokenize failure");
			return (FAILURE);//free
		}

		/* input validation (Mandatory/Bonus) */
		if (!is_return_input && arrange_and_validate_token_list(&info->tokenlist_head) == FAILURE)
			is_return_input = true;//add_history & free(line)

		debug_print_token_word(info->tokenlist_head, "arranged");

		/* parsing (Mandatory/Bonus) */
		if (!is_return_input && parsing_token_list(info) == FAILURE)
		{
			ft_dprintf(STDERR_FILENO, "parsing failure");
			return (FAILURE);
		}

		/* expansion */
		/* command_execution */

		add_history(input_line);
		input_line = free_1d_alloc(input_line);
		clear_input(&info);
	}

	return (exit_status);
}
