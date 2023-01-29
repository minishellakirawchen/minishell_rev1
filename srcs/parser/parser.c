/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:48 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 12:57:24 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	delete_last_semicolon_node(t_exec_list **exec_list_head)
{
	t_exec_list	*exec_list;

	if (!exec_list_head || !*exec_list_head)
		return ;
	exec_list = *exec_list_head;
	while (exec_list->next)
		exec_list = exec_list->next;
	if (exec_list->node_kind != e_node_semicolon)
		return ;
	if (!exec_list->prev)
		return ;
	exec_list->prev->next = NULL;
	clear_exec_list(&exec_list);
}

int	parsing_token_list(t_info *info)
{
	if (!info || !info->tokenlist_head)
		return (FAILURE);
	/* operator list */
	if (create_operator_list(info) == FAILURE)
	{
		ft_dprintf(STDERR_FILENO, "fail to create_operator_list\n");
		return (FAILURE);
	}
//	debug_print_exec_list(info->execlist_head, "operator_list");

	delete_last_semicolon_node(&info->execlist_head);

//	debug_print_exec_list(info->execlist_head, "delete last ;");

	if (create_command_list(&info->execlist_head) == FAILURE)
	{
		ft_dprintf(STDERR_FILENO, "fail to create_command_list\n");
		return (FAILURE);
	}

	//quote_removal_in_heredoc, exec_heredocのために""の結合とremovalが必要
	//redirect_listをまず作成、fileのexpandはあとで実施（heredoc eofはexpandなし）

	// fileのexpansionをあとで実行すると, "'$key'" vs '$key'の判別がつかなくなる
	// redirect_listを作成し、redirect_info->token_listに保管しておく
	// heredocは結合してheredoc_eofを作成する
	// fileの展開、結合はあとで実施する
	if (create_redirect_list(&info->execlist_head) == FAILURE)
		return (FAILURE);

	//	debug_print_exec_list(info->execlist_head, "command_list");

	return (SUCCESS);
}

//             [root]
//               ┃
//             [&&]
// ┏━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┓
//[a]                         [sub-shell]
//                  ┏━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┓
//                [&&]                           [&&]
//       ┏━━━━━━━━━━┻━━━━━━━━━━━┓             ┏━━━┻━━━━┓
//  [sub-shell]            [sub-shell]       [g]       null
//  ┏━━━━┻━━━━┓           ┏━━━━━┻━━━━━━┓
//[b]        null       [||]           null
//                   ┏━━━┻━━━━┓
//                  [c]      [|]
//                        ┏━━━┻━━━━┓
//                       [d]   [sub-shell]
//                             ┏━━━┻━━━━┓
//                            [f]       [f]

/*
size_t	count_subshell_in_height(t_list *list_node, int search_height)
{
	size_t			cnt;
	t_token_elem	*token;

	if (!list_node)
		return (0);
	cnt = 0;
	while (list_node)
	{
		token = list_node->content;
		if (token->type == e_subshell_start && token->depth == search_height)
			cnt++;
		list_node = list_node->next;
	}
	return (cnt);
}

t_node_kind set_node_kind(t_token_type token_type)
{
	if (is_tokentype_semicolon(token_type) || is_tokentype_pipe_or_and(token_type))
		return (e_node_operator);
	if (is_tokentype_subshell(token_type))
		return (e_node_subshell);
	return (e_node_pipeline);
}

char	*get_node_kind(t_node_kind kind)
{
	if (kind == e_node_operator)
		return ("operator");
	if (kind == e_node_subshell)
		return ("subshell");
	return ("pipeline_commands");
}
*/

