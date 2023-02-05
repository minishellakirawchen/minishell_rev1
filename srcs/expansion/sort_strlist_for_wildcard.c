/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_strlist_for_wildcard.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 13:01:24 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:02:05 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static void	swap(void **content_a, void **content_b)
{
	void	*tmp;

	if (!content_a || !content_b)
		return ;
	tmp = *content_a;
	*content_a = *content_b;
	*content_b = tmp;
}

void	sort_ascending_strlist(t_list_bdi **list_head)
{
	t_list_bdi		*node_a;
	t_list_bdi		*node_b;
	t_token_elem	*token_a;
	t_token_elem	*token_b;

	if (!list_head || !*list_head)
		return ;
	node_a = *list_head;
	while (node_a)
	{
		node_b = node_a->next;
		while (node_b)
		{
			token_a = node_a->content;
			token_b = node_b->content;
			if (ft_strcmp_ns(token_a->word, token_b->word) > 0)
				swap(&node_a->content, &node_b->content);
			node_b = node_b->next;
		}
		node_a = node_a->next;
	}
}
