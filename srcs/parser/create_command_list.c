/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:02:36 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 08:56:33 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

static int	connect_cmd_info_to_execlist(
		t_list_bdi **connect_to, t_command_info *command_info)
{
	t_list_bdi	*new_pipeline;

	new_pipeline = ft_lstnew_bdi(command_info);
	if (!new_pipeline)
		return (FAILURE);
	ft_lstadd_back_bdi(connect_to, new_pipeline);
	return (SUCCESS);
}

static t_command_info	*create_command_info(void)
{
	t_command_info	*command_info;

	errno = 0;
	command_info = (t_command_info *)malloc(sizeof(t_command_info));
	if (!command_info)
		return (perror_ret_nullptr("malloc"));
	command_info->type = e_node_init;
	command_info->commands = NULL;
	command_info->redirect_list = NULL;
	command_info->redirect_fd[FD_INFILE] = -1;
	command_info->redirect_fd[FD_OUTFILE] = -1;
	command_info->redirect_fd[FD_HEREDOC] = -1;
	command_info->pid = -1;
	command_info->pipeline_token_list = NULL;
	command_info->subshell_token_list = NULL;
	return (command_info);
}

static int	create_cmd_info_from_pipeline(\
t_exec_list **pipe_node, t_command_info *cmd_info)
{
	t_token_elem	*token_elem;
	t_list_bdi		*popped_token;

	if (!pipe_node || !*pipe_node || !(*pipe_node)->token_list_head)
		return (FAILURE);
	while ((*pipe_node)->token_list_head)
	{
		popped_token = ft_lstpop_bdi(&(*pipe_node)->token_list_head);
		token_elem = popped_token->content;
		if (token_elem->type != e_ope_pipe)
		{
			move_tokens_to_cmd_info(\
			&(*pipe_node)->token_list_head, &cmd_info, popped_token);
			continue ;
		}
		if (connect_cmd_info_to_execlist(\
		&(*pipe_node)->pipeline_commands, cmd_info) == FAILURE)
			return (FAILURE);
		ft_lstdelone_bdi(&popped_token, free_token_elem);
		cmd_info = create_command_info();
		if (!cmd_info)
			return (FAILURE);
	}
	return (SUCCESS);
}

/*
 * skip if node is operator
 * otherwise, create list = create_command_list(node->tokenlist)
 * connect next list as `pipe`
 * if subshell, assign in subshell list, parsing later process.
 */
int	create_command_list(t_exec_list **exec_list_head)
{
	t_exec_list		*node;
	t_command_info	*cmd_info;

	if (!exec_list_head || !*exec_list_head)
		return (FAILURE);
	node = *exec_list_head;
	while (node)
	{
		if (node->node_kind == e_node_pipeline)
		{
			cmd_info = create_command_info();
			if (!cmd_info)
				return (FAILURE);
			if (create_cmd_info_from_pipeline(&node, cmd_info) == FAILURE)
				return (FAILURE);
			if (connect_cmd_info_to_execlist(\
			&node->pipeline_commands, cmd_info) == FAILURE)
				return (FAILURE);
		}
		node = node->next;
	}
	return (SUCCESS);
}
