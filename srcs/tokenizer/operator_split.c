/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 17:15:53 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:44:48 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/*
 ["hello world"]=['good bye']=[|cat]->[Makefile>out]=['hoge']
                         keep^ ^^             ^^^   ^keep
                               split         split
*/

static t_list_bdi	*get_last_splitted_node(\
t_token_elem *token_elem, t_list_bdi **prev, t_list_bdi **token_head)
{
	t_list_bdi		*splitted_list_head;
	t_list_bdi		*last_node;

	if (!token_elem || !prev || !token_head)
		return (NULL);
	splitted_list_head = get_split_before_after_opes(\
	token_elem->word, STR_OPERATOR, STR_SPACE);
	if (!splitted_list_head)
		return (NULL);
	if (*prev)
		(*prev)->next = splitted_list_head;
	else
		*token_head = splitted_list_head;
	last_node = ft_lstlast_bdi(splitted_list_head);
	return (last_node);
}

static void	handle_tokens(t_token_elem **last_token, t_list_bdi **last_node, \
t_list_bdi **now_node, t_token_elem *token_elem)
{
	if (!last_token || !last_node || !now_node || !token_elem)
		return ;
	*last_token = (*last_node)->content;
	(*last_token)->is_connect_to_next = token_elem->is_connect_to_next;
	(*last_node)->next = (*now_node)->next;
	ft_lstdelone_bdi(now_node, free_token_elem);
	*now_node = *last_node;
}

int	split_by_operators(t_list_bdi **token_head)
{
	t_list_bdi		*now_node;
	t_list_bdi		*prev;
	t_list_bdi		*last_node;
	t_token_elem	*token_elem;
	t_token_elem	*last_token;

	if (!token_head || !*token_head)
		return (FAILURE);
	now_node = *token_head;
	prev = NULL;
	while (now_node)
	{
		token_elem = now_node->content;
		if (!token_elem->is_quoted \
		&& is_str1chrs_in_str2(STR_OPERATOR, token_elem->word))
		{
			last_node = get_last_splitted_node(token_elem, &prev, token_head);
			if (!last_node)
				return (FAILURE);
			handle_tokens(&last_token, &last_node, &now_node, token_elem);
		}
		prev = now_node;
		now_node = now_node->next;
	}
	return (SUCCESS);
}
