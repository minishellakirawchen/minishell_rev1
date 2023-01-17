/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:55 by takira            #+#    #+#             */
/*   Updated: 2023/01/17 18:41:22 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

int	prompt_loop(t_info *info)
{
	int		exit_status;
	char	*input_line;

	if (!info)
		return (FAILURE);
	exit_status = EXIT_SUCCESS;
	while (true)
	{
		input_line = readline(PROMPT);
		if (!input_line)
			break ;
		if (is_same_str(input_line, NL))
		{
			input_line = free_1d_alloc(input_line);
			ft_dprintf(STDERR_FILENO, "NL only, Not add to history");
			continue ;
		}
		ft_dprintf(STDERR_FILENO, "input:%s\n", input_line);
		// tokenize

		// parsing

		// expansion

		// command_execution

		add_history(input_line);
		input_line = free_1d_alloc(input_line);
	}

	return (exit_status);
}
