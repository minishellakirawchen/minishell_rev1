/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:13 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 11:15:08 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/* command_list->redirect_list = heredoc->io->io->heredoc->... */
/* if type=io, expand and create filename
 * from redirect_list->content=redirect_info->token_list */
int	expand_var_and_create_cmds_from_tokens(\
t_exec_list **pipeline_node, t_info *info)
{
	t_list_bdi		*cmd_list_node;
	t_command_info	*cmd_info;

	if (!pipeline_node || !info)
		return (FAILURE);
	cmd_list_node = (*pipeline_node)->pipeline_commands;
	while (cmd_list_node)
	{
		cmd_info = cmd_list_node->content;
		if (cmd_info->pipeline_token_list)
		{
			cmd_info->commands = create_expanded_commands(\
			&cmd_info->pipeline_token_list, info, NULL);
			if (!cmd_info->commands)
				return (FAILURE);
		}
		if (expand_var_in_redirect_filename(&cmd_info, info) == FAILURE)
			return (FAILURE);
		cmd_list_node = cmd_list_node->next;
	}
	return (SUCCESS);
}
