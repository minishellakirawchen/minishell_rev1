/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:22:41 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 14:49:58 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

// pipeline_token_list
// expand & split -> append expanded_tokens

int	create_commands_from_pipeline_tokens(t_command_info **cmd_list, t_info *info)
{
//	t_list_bdi	*expanded_tokens;
//	t_list_bdi	*popped_node;
	char		*commands;

	if (!cmd_list || !*cmd_list || !info)
		return (FAILURE);
	commands = NULL;
	/*
	// expand -> quote removal -> space split -> add expanded_tokens;
	expanded_tokens = NULL;
	while ((*cmd_list)->pipeline_token_list)
	{
		popped_node = ft_lstpop_bdi(&(*cmd_list)->pipeline_token_list);



	}
*/
	// expanded_tokens ->

	(*cmd_list)->commands = (char **)commands;
	return (SUCCESS);
}
