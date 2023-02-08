/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:32:12 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 16:32:21 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

static bool	is_empty_parenthesis(t_list_bdi *token_list)
{
	t_token_elem	*first_token;
	t_token_elem	*second_token;

	if (ft_lstsize_bdi(token_list) != 2)
		return (false);
	first_token = token_list->content;
	second_token = token_list->next->content;
	if (is_same_str(first_token->word, "(") \
	&& is_same_str(second_token->word, ")"))
		return (true);
	return (false);
}

int	execute_subshell(t_list_bdi **token_list, t_info *info)
{
	t_exec_list	*exec_list;
	int			exit_status;

	if (!token_list || !info)
		return (PROCESS_ERROR);
	if (is_empty_parenthesis(*token_list))
		return (EXIT_FAILURE);
	exec_list = NULL;
	if (parsing_token_list(token_list, &exec_list, info) == PROCESS_ERROR)
	{
		clear_exec_list(&exec_list);
		return (PROCESS_ERROR);
	}
	exit_status = execute_execlist(&exec_list, info);
	clear_exec_list(&exec_list);
	return (exit_status);
}
