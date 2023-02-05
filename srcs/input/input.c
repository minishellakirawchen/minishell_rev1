/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:55 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 22:01:39 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gbl_var g_var;

static void prompt_int_handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		g_var.exit_status = EXIT_FAILURE;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	init_signal_prompt(void)
{
	struct sigaction	sig_int_act;
	struct sigaction	sig_quit_act;

	signal(SIGINT, SIG_IGN);
	ft_bzero(&sig_int_act, sizeof(sigaction));
	init_sigaction(SIGINT, sig_int_act, prompt_int_handler);
	ft_bzero(&sig_quit_act, sizeof(sigaction));
	init_sigaction(SIGQUIT, sig_quit_act, SIG_IGN);
}

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
	//int		exit_status;
	bool	is_return_input;
	char	*prompt;
	char	*tmp;
	char	*exit_status_string;

	if (!info)
		return (FAILURE);
	while (true)
	{
		init_signal_prompt();
		exit_status_string = ft_itoa(info->exit_status);
		tmp = ft_strjoin("minishell ", exit_status_string);
		prompt = ft_strjoin(tmp, " $> ");
		free(tmp);
		free(exit_status_string);

		is_return_input = false;
		/* input */
		set_tc_attr_out_execute();
		info->readline_input = readline(prompt);
		free (prompt);
		if (!info->readline_input)
		{
			// debug
			ft_dprintf(STDERR_FILENO, "exit");
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
		g_var.exit_status = tokenize_input_line(info, info->readline_input);
		is_return_input |= g_var.exit_status;
		if (g_var.exit_status != EXIT_SUCCESS)
			ft_dprintf(STDERR_FILENO, "[#DEBUG]tokenize failure\n");

		/* input validation (Mandatory/Bonus) */
		if (g_var.exit_status == EXIT_SUCCESS)
		{
			g_var.exit_status = arrange_and_validate_token_list(&info->tokenlist_head);
			is_return_input |= g_var.exit_status;
		}
		// debug
		debug_print_tokens(info->tokenlist_head, "arranged");

		/* parsing (Mandatory/Bonus) */
		if (!is_return_input && g_var.exit_status == EXIT_SUCCESS)
			g_var.exit_status = parsing_token_list(&info->tokenlist_head, &info->execlist_head, info);
		is_return_input |= g_var.exit_status;
		if (g_var.exit_status != EXIT_SUCCESS)
			ft_dprintf(STDERR_FILENO, "[#DEBUG]parsing failure\n");
		/* expansion & command_execution */
		if (!is_return_input && g_var.exit_status == EXIT_SUCCESS)
			g_var.exit_status = execute_execlist(&info->execlist_head, info);

		/* clear input */
		clear_input_info(&info);
		if (g_var.exit_status == PROCESS_ERROR)
			break ;
		info->exit_status = g_var.exit_status;
		// exit_status_string = ft_itoa(exit_status);
		// tmp = ft_strjoin("minishell ", exit_status_string);
		// prompt = ft_strjoin(tmp, " $> ");
		// free(tmp);
		// free(exit_status_string);
	}
	return (g_var.exit_status);
}
