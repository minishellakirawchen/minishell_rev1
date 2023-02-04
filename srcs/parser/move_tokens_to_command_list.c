/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_tokens_to_cmd_info.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:12:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 08:54:32 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	move_tokens_to_cmd_info(\
t_list_bdi **tok_lst, t_command_info **cmd_lst, t_list_bdi *popped_tok)
{
	t_token_elem	*token_elem;

	if (!tok_lst || !cmd_lst || !popped_tok)
		return ;
	token_elem = popped_tok->content;
	if (is_tokentype_subshell(token_elem->type))
		move_tokens_to_subshell_list(tok_lst, cmd_lst, popped_tok);
	else
		move_tokens_to_pipeline_list(cmd_lst, popped_tok);
}

void	move_tokens_to_subshell_list(
		t_list_bdi **tok_lst, t_command_info **cmd_lst, t_list_bdi *popped_tok)
{
	t_token_elem	*token_elem;
	ssize_t			subshell_depth;

	if (!tok_lst || !cmd_lst || !popped_tok)
		return ;
	token_elem = popped_tok->content;
	(*cmd_lst)->type = e_node_subshell;
	subshell_depth = token_elem->subshell_depth;
	ft_lstdelone_bdi(&popped_tok, free_token_elem);
	while (*tok_lst)
	{
		popped_tok = ft_lstpop_bdi(&(*tok_lst));
		token_elem = popped_tok->content;
		if (token_elem->type == e_subshell_end \
		&& token_elem->subshell_depth == subshell_depth)
		{
			ft_lstdelone_bdi(&popped_tok, free_token_elem);
			return ;
		}
		ft_lstadd_back_bdi(&(*cmd_lst)->subshell_token_list, popped_tok);
	}
}

void	move_tokens_to_pipeline_list(
		t_command_info **command_list, t_list_bdi *popped_token)
{
	if (!command_list || !popped_token)
		return ;
	if ((*command_list)->type == e_node_init)
		(*command_list)->type = e_node_pipeline;
	ft_lstadd_back_bdi(&(*command_list)->pipeline_token_list, popped_token);
}
