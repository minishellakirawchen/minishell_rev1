/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:55 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 21:10:48 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

volatile sig_atomic_t	g_signal_status = EXIT_SUCCESS;

static void	prompt_int_handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		g_signal_status = EXIT_FAILURE;
		write(STDOUT_FILENO, "\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	init_signal_prompt(void)
{
	struct sigaction	sig_int_act;
	struct sigaction	sig_quit_act;

	signal(SIGINT, SIG_IGN);
	ft_bzero(&sig_int_act, sizeof(sigaction));
	init_sigaction(SIGINT, sig_int_act, prompt_int_handler);
	ft_bzero(&sig_quit_act, sizeof(sigaction));
	init_sigaction(SIGQUIT, sig_quit_act, SIG_IGN);
}

int	preprocess_for_execute(t_info *info)
{
	int	process_exit_val;

	if (!info->readline_input)
	{
		ft_dprintf(STDERR_FILENO, "exit");
		return (BREAK);
	}
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

void	prompt_init(int *process_exit_value, t_info *info)
{
	*process_exit_value = EXIT_SUCCESS;
	set_tc_attr_out_execute();
	info->readline_input = readline(PROMPT);
	set_tc_attr_in_execute();
}

int	prompt_loop(t_info *info)
{
	int	process_exit_value;

	if (!info)
		return (FAILURE);
	init_signal_prompt();
	while (true)
	{
		prompt_init(&process_exit_value, info);
		process_exit_value = preprocess_for_execute(info);
		if (process_exit_value == CONTINUE)
			continue ;
		if (process_exit_value == BREAK)
			break ;
		if (g_signal_status == EXIT_FAILURE)
			info->exit_status = g_signal_status;
		if (process_exit_value == EXIT_SUCCESS)
			process_exit_value = execute_execlist(&info->execlist_head, info);
		clear_input_info(&info);
		if (process_exit_value == PROCESS_ERROR)
			break ;
		info->exit_status = process_exit_value;
		g_signal_status = EXIT_SUCCESS;
	}
	return (g_signal_status);
}
