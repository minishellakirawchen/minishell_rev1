/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:55 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 16:41:59 by takira           ###   ########.fr       */
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
	char	*prompt;
	char	*tmp;
	char	*exit_status_string;

	if (!info)
		return (FAILURE);
	exit_status = EXIT_SUCCESS;
	while (true)
	{
		exit_status_string = ft_itoa(exit_status);
		tmp = ft_strjoin("minishell ", exit_status_string);
		prompt = ft_strjoin(tmp, " $> ");
		free(tmp);
		free(exit_status_string);

		is_return_input = false;
		/* input */
		info->readline_input = readline(prompt);
		free(prompt);

//		info->readline_input = readline("minishell $ ");
//		info->readline_input = readline(PROMPT);
		if (!info->readline_input)
		{
			// debug
			ft_dprintf(STDERR_FILENO, "[#DEBUG]^D Pressed. Exit minishell\n");  // check exit_status using ^D
			break ;
		}
		if (is_same_str(info->readline_input, ""))
		{
			info->readline_input = free_1d_alloc(info->readline_input); // Not update exit_status
			continue ;
		}
		add_history(info->readline_input); // `space` includes

		// debug
//		ft_dprintf(STDERR_FILENO, "#%-15s:[%s]\n", "input", info->readline_input);

		/* tokenize */
		exit_status = tokenize_input_line(info, info->readline_input);
		is_return_input |= exit_status;
		if (exit_status != EXIT_SUCCESS)
			ft_dprintf(STDERR_FILENO, "[#DEBUG]tokenize failure\n");

		/* input validation (Mandatory/Bonus) */
		exit_status = arrange_and_validate_token_list(&info->tokenlist_head);
		is_return_input |= exit_status;

		// debug
		debug_print_tokens(info->tokenlist_head, "arranged");

		/* parsing (Mandatory/Bonus) */
		if (!is_return_input)
			exit_status = parsing_token_list(&info->tokenlist_head, &info->execlist_head, info);
		is_return_input |= exit_status;
		if (exit_status != EXIT_SUCCESS)
			ft_dprintf(STDERR_FILENO, "[#DEBUG]parsing failure\n");

		/* expansion & command_execution */
		if (!is_return_input)
			exit_status = execute_execlist(&info->execlist_head, info);

		/* clear input */
		clear_input_info(&info);
		if (exit_status == PROCESS_ERROR)
			break ;
		info->exit_status = exit_status;
	}
	return (exit_status);
}
