/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:48 by takira            #+#    #+#             */
/*   Updated: 2023/01/22 16:31:46 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "paeser.h"

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
		return (e_operator);
	if (is_tokentype_subshell(token_type))
		return (e_subshell);
	return (e_pipeline);
}

char	*get_node_kind(t_node_kind kind)
{
	if (kind == e_operator)
		return ("operator");
	if (kind == e_subshell)
		return ("subshell");
	return ("pipeline");
}

t_exec_list *create_execlist_node(t_node_kind kind, t_list *token_head, t_exec_list **prev, t_exec_list **next)
{
	t_exec_list	*new_node;
	t_exec_list	*set_prev;
	t_exec_list	*set_next;

	errno = 0;
	new_node = (t_exec_list *)malloc(sizeof(t_exec_list));
	if (!new_node)
	{
		perror("malloc");
		return (NULL);
	}
	set_prev = NULL;
	set_next = NULL;
	if (prev && *prev)
	{
		set_prev = *prev;
		set_prev->next = new_node;
	}
	if (next && *next)
		set_next = *next;
	new_node->node_kind = kind;
	new_node->token_list_head = token_head;
	new_node->prev = set_prev;
	new_node->next = set_next;
	return (new_node);
}


// echo hello | echo world && cat Makefile | grep a || (cd /bin && ls) > out; cat <infile;
// A && B || C ; D
// [root]
//   |
//  [pipeline]:echo hello | echo world
//  [&&]
//  [pipeline]:echo hello | echo world

// 初めはpipelineを作る
// operatorがきたらpipeline->operator


t_exec_list	*create_operator_list(t_list **tokenlist_head)
{
	t_list			*token_node;
	t_list			*next_node;
	t_list			*popped_node;
	t_exec_list		*operator_node;
	t_exec_list		*exec_head;
	t_exec_list		*pipeline_node;
	t_token_elem	*token_elem;

	if (!tokenlist_head || !*tokenlist_head)
		return (NULL);

	exec_head = create_execlist_node(e_head, NULL, NULL, NULL);
	pipeline_node = create_execlist_node(e_pipeline, NULL, &exec_head, NULL);
	if (!exec_head || !pipeline_node)
		return (NULL); //TODO:all free
	token_node = *tokenlist_head;
	while (token_node)
	{
		token_elem = token_node->content;
		next_node = token_node->next;
		if (is_tokentype_pipeline(token_elem->type) || token_elem->depth > 0)
		{
			popped_node = token_node;
			popped_node->next = NULL;
			ft_lstadd_back(&(pipeline_node->token_list_head), popped_node);
		}
		else
		{
			operator_node = create_execlist_node(e_operator, NULL, &pipeline_node, NULL);
			if (!operator_node)
				return (perror_ret_nullptr("malloc")); // TODO:all free
			operator_node->token_type = token_elem->type;
			pipeline_node = create_execlist_node(e_pipeline, NULL, &operator_node, NULL);
			if (!pipeline_node)
				return (perror_ret_nullptr("malloc")); // TODO:all free
			ft_lstdelone(token_node, free_token_elem);
		}
		token_node = next_node;
	}
	return (exec_head);
}

int	create_command_list(t_exec_list **exec_list_head)
{
	if (!exec_list_head || !*exec_list_head)
		return (FAILURE);


	return (SUCCESS);
}

int	parsing_token_list(t_info *info)
{
	t_exec_list	*exec_list_head;

	if (!info || !info->tokenlist_head)
		return (FAILURE);

	// operator list
	exec_list_head= create_operator_list(&info->tokenlist_head);
	if (!exec_list_head)
		return (FAILURE);
	// print
	debug_print_exec_list(exec_list_head, "operator_list");




	// tokenlist->tree
	// return
	return (SUCCESS);
}
































