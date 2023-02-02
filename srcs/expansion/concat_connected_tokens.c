/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_connected_tokens.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 10:36:47 by takira            #+#    #+#             */
/*   Updated: 2023/02/02 13:29:11 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

void	get_connected_tokens(t_list_bdi **list_connected_from, t_list_bdi **list)
{
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;

	if (!list_connected_from)
		return ;
	popped_node = ft_lstpop_bdi(list_connected_from);
	ft_lstadd_back_bdi(list, popped_node);
	token_elem = popped_node->content;
	if (token_elem->is_connect_to_next_word)
		get_connected_tokens(list_connected_from, list);
}

bool	check_wildcard_quoted(t_list_bdi *token_node)
{
	t_token_elem	*token_elem;
	if (!token_node)
		return (false);
	token_elem = token_node->content;
	return (is_expandable_wildcard_in_str(token_elem->word, token_elem->is_quoted));
}

int	concat_connected_tokens(t_list_bdi **token_list)
{
	t_list_bdi		*node;
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;
	t_token_elem 	*append_elem;

	if (!token_list)
		return (FAILURE);
	node = *token_list;
	while (node)
	{
		token_elem = node->content;
		token_elem->is_wildcard_quoted = check_wildcard_quoted(node);
		node = node->next;
		if (!token_elem->is_connect_to_next_word)
			continue ;
		while (node)
		{
			popped_node = ft_lstpop_bdi(&node);
			append_elem = popped_node->content;
			token_elem->word = concat_dst_to_src(&token_elem->word, append_elem->word);
			if (!token_elem->word)
				return (perror_ret_int("malloc", FAILURE)); //TODO:free
			token_elem->is_wildcard_quoted = check_wildcard_quoted(popped_node);
			if (!append_elem->is_connect_to_next_word)
				break ;
			ft_lstclear_bdi(&popped_node, free_token_elem);
		}
		ft_lstclear_bdi(&popped_node, free_token_elem);
		token_elem->is_connect_to_next_word = false;
	}
	return (SUCCESS);
}
