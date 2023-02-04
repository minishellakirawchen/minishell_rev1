/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 23:15:59 by takira            #+#    #+#             */
/*   Updated: 2023/02/02 17:23:47 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

// "$hoge $huga"
// remove "'	//"'hoge'"->'hoge'

// $hoge$huga -> helloworld$huga
// ^^^^^         ^^^^^^^^^^
// ^idx                    ^idx
// pop->addback newlist

int expand_var_in_token_word(t_list_bdi **src_tokens, t_info *info)
{
	t_token_elem	*token_elem;
	t_list_bdi		*node;

	if (!src_tokens || !info)
		return (FAILURE);

	node = *src_tokens;
	while (node)
	{
		token_elem = node->content;
		if (is_expandable_var(token_elem->word, token_elem->quote_chr))
			if (expand_var_in_str(&token_elem->word, info) == FAILURE)
				return (FAILURE);
		node = node->next;
	}
	return (SUCCESS);
}

char	*create_string_by_concat_tokens(t_list_bdi *list_head)
{
	char			*concat_str;
	t_list_bdi		*node;
	t_token_elem	*token_elem;
	size_t			size;

	if (!list_head)
		return (NULL);
	size = 0;
	node = list_head;
	while (node)
	{
		token_elem = node->content;
		size += ft_strlen_ns(token_elem->word);
		node = node->next;
	}
	concat_str = (char *)ft_calloc(sizeof(char), size + 1);
	if (!concat_str)
		return (perror_ret_nullptr("malloc"));
	node = list_head;
	while (node)
	{
		token_elem = node->content;
		ft_strlcat(concat_str, token_elem->word, size + 1);
		node = node->next;
	}
	return (concat_str);
}

