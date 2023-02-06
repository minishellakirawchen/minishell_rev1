/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:55 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 17:42:52 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gbl_var	g_status;

void	prompt_int_handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		g_status.exit_status = EXIT_FAILURE;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static int	preprocess_for_execute(t_info *info)
{
	int	process_exit_val;

	if (!info->readline_input)
		return (BREAK);
	if (is_same_str(info->readline_input, ""))
	{
		info->readline_input = free_1d_alloc(info->readline_input);
		return (CONTINUE);
	}
	add_history(info->readline_input);
	process_exit_val = tokenize_input_line(info, info->readline_input);
	if (process_exit_val == CONTINUE)
		return (CONTINUE);
	if (process_exit_val == EXIT_SUCCESS)
		process_exit_val = arrange_and_validate_token_list(\
		&info->tokenlist_head);
	if (process_exit_val == EXIT_SUCCESS)
		process_exit_val = parsing_token_list(\
				&info->tokenlist_head, &info->execlist_head, info);
	return (process_exit_val);
}

static void	prompt_init(int *process_exit_value, t_info *info)
{
	if (*process_exit_value == EXIT_BY_SIG) //-4->1
		info->exit_status = EXIT_FAILURE;
	else if (*process_exit_value != CONTINUE)
		info->exit_status = *process_exit_value;
	*process_exit_value = EXIT_SUCCESS;
	g_status.exit_status = EXIT_SUCCESS;
	set_tc_attr_out_execute();
	init_signal_prompt();
//	set_tc_attr_in_execute();
//	info->readline_input = readline(PROMPT);

////////////////////////////////////////////////////////////////////
	char	*prompt;
	char	*tmp;
	char	*exit_status_string;
	exit_status_string = ft_itoa(info->exit_status);
	tmp = ft_strjoin("minishell ", exit_status_string);
	prompt = ft_strjoin(tmp, " $> ");
	free(tmp);
	free(exit_status_string);

	info->readline_input = readline(prompt);
	free(prompt);
////////////////////////////////////////////////////////////////////

}

static void	update_exit_status(t_info *info)
{
	if (g_status.exit_status == EXIT_FAILURE)
		info->exit_status = g_status.exit_status;
}

int	prompt_loop(t_info *info)
{
	int	process_exit_value;

	if (!info)
		return (FAILURE);
//	init_signal_prompt();
	process_exit_value = EXIT_SUCCESS;
	while (true)
	{
		prompt_init(&process_exit_value, info);
		process_exit_value = preprocess_for_execute(info);
		if (is_eof_exit(process_exit_value))
			break ;
		update_exit_status(info);
		if (process_exit_value == EXIT_SUCCESS)
			process_exit_value = execute_execlist(&info->execlist_head, info);
		clear_input_info(&info);
		if (is_minishell_abort(process_exit_value))
			break ;
	}
	return (info->exit_status);
}
