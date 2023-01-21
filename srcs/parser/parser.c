/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:48 by takira            #+#    #+#             */
/*   Updated: 2023/01/21 15:08:14 by takira           ###   ########.fr       */
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
		if (token->type == e_subshell_start && token->parenthesis_no == search_height)
			cnt++;
		list_node = list_node->next;
	}
	return (cnt);
}

t_tree_node_type	set_tree_type(t_token_type token_type)
{
	if (is_tokentype_semicolon(token_type) || is_tokentype_pipe_or_and(token_type))
		return (e_operator);
	if (is_tokentype_subshell(token_type))
		return (e_subshell);
	return (e_command_line);
}

t_tree *create_tree_node(t_tree_node_type type, t_list *token_head)
{
	t_tree	*new_node;

	errno = 0;
	new_node = (t_tree *)malloc(sizeof(t_tree));
	if (!new_node)
	{
		perror("malloc");
		return (NULL);
	}
	new_node->tree_type = type;
	new_node->token_list_head = token_head;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_tree	*create_tree(t_list *tokenlist_head)
{
	t_tree			*tree_root;
	t_list			*list_node;
	t_list			*section_list_head;
	t_token_elem	*token_elem;
	t_tree			*new_operator_node;
	t_tree			*new_list_node;

	tree_root = create_tree_node(e_root, NULL);
	if (!tree_root)
		return (NULL);

	list_node = tokenlist_head;
	section_list_head = tokenlist_head;
	new_list_node = NULL;
	while (list_node)
	{
		token_elem = list_node->content;
		// operator: create list_node and append tree_root
		if (is_tokentype_list_operator(token_elem->type))
		{
			// section_list_head->...->list_node->NULL
			new_operator_node = create_tree_node(set_tree_type(token_elem->type), section_list_head);
			if (!new_operator_node)
				return (NULL); //TODO:free
			new_operator_node->left = new_list_node;

			// add new_tree to root
			add_bottom_of_tree(&tree_root, new_operator_node);

			//init setction_list_head
			new_operator_node = NULL; //free?
			new_list_node = NULL;
		}
		else
		{
			// command_list & readirection list
			// make command list
			// handle redirect information



		}
		list_node = list_node->next;
	}
	return (tree_root);
}


// treeを作る
int	parsing_token_list(t_info *info)
{
	t_tree	*tree_root;

	tree_root = create_tree();

	// tokenlist->tree
	// return
}



































