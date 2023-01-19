/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 10:09:06 by takira            #+#    #+#             */
/*   Updated: 2023/01/19 13:18:00 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/*
int validate_tokens_for_mandatory(t_info *info)
{
	t_list			*list_node;
	t_list			*next_list;
	t_token_elem	*token_elem;
	t_token_elem	*next_token;

	if (!info || !info->tokenlist_head)
		return (FAILURE);
	list_node = info->tokenlist_head;
	while (list_node)
	{
		// delete connection to control opes
		// NG : continue control opes, duplicate control opes
		token_elem = list_node->content;

		list_node = list_node->next;
	}
	return (SUCCESS);
}
 */