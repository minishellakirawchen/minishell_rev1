/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_tokens_to_command_list.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:12:32 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 10:18:12 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	move_tokens_to_subshell_list(t_list_bdi **token_list, t_command_info **command_list, t_list_bdi *popped_token);
static void	move_tokens_to_pipeline_list(t_command_info **command_list, t_list_bdi *popped_token);

void	move_tokens_to_command_list(t_list_bdi **token_list, t_command_info **command_list, t_list_bdi *popped_token)
{
	t_token_elem	*token_elem;

	if (!token_list || !command_list || !popped_token)
		return ;
	token_elem = popped_token->content;
	if (is_tokentype_subshell(token_elem->type))
		move_tokens_to_subshell_list(token_list, command_list, popped_token);
	else
		move_tokens_to_pipeline_list(command_list, popped_token);
}

void	move_tokens_to_subshell_list(t_list_bdi **token_list, t_command_info **command_list, t_list_bdi *popped_token)
{
	t_token_elem	*token_elem;
	ssize_t			subshell_depth;

	if (!token_list || !command_list || !popped_token)
		return ;
	token_elem = popped_token->content;
	(*command_list)->type = e_node_subshell;
	subshell_depth = token_elem->depth;
	ft_lstdelone_bdi(&popped_token, free_token_elem); // delete '('
	while (*token_list)
	{
		popped_token = ft_lstpop_bdi(&(*token_list));
		token_elem = popped_token->content;
		if (token_elem->type == e_subshell_end && token_elem->depth == subshell_depth)
		{
			ft_lstdelone_bdi(&popped_token, free_token_elem); // delete ')'
			return ;
		}
		ft_lstadd_back_bdi(&(*command_list)->subshell_token_list, popped_token);
	}
}

void	move_tokens_to_pipeline_list(t_command_info **command_list, t_list_bdi *popped_token)
{
	if (!command_list || !popped_token)
		return ;
	if ((*command_list)->type == e_node_init)
		(*command_list)->type = e_node_pipeline;
	ft_lstadd_back_bdi(&(*command_list)->pipeline_token_list, popped_token);
}
