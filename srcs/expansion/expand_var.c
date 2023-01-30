/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 23:15:59 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 21:34:35 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"



// "$hoge $huga"
// remove "'	//"'hoge'"->'hoge'

// $hoge$huga -> helloworld$huga
// ^^^^^         ^^^^^^^^^^
// ^idx                    ^idx
// pop->addback newlist

// mendokusai...

int	expand_var_in_tokens(t_list_bdi **list_head, t_info *info)
{
	t_list_bdi		*list_node;
	t_token_elem	*token_elem;

	if (!list_head || !*list_head || !info)
		return (SUCCESS);

	list_node = *list_head;
	while (list_node)
	{
		token_elem = list_node->content;
		if (is_expandable_var_in_str(token_elem->word, token_elem->quote_chr))
		{
			token_elem->word = get_expanded_str(token_elem->word, info);
			if (!token_elem->word)
				return (FAILURE);
		}
//		if (token_elem->is_quoted)
//			if (remove_quotes(&token_elem->word) == FAILURE)
//				return (FAILURE);
		list_node = list_node->next;
	}
	return (SUCCESS);
}

int	remove_quote_in_tokens(t_list_bdi **list_head)
{
	t_list_bdi		*list_node;
	t_token_elem	*token_elem;

	if (!list_head || !*list_head)
		return (SUCCESS);

	list_node = *list_head;
	while (list_node)
	{
		token_elem = list_node->content;
		if (token_elem->is_quoted)
			if (remove_quotes(&token_elem->word) == FAILURE)
				return (FAILURE);
		list_node = list_node->next;
	}
	return (SUCCESS);
}

char	*concat_tokens(t_list_bdi *list_head)
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

int remove_quotes(char **token_word)
{
	size_t	wordlen;
	char	*quote_removal_word;

	if (!token_word || !*token_word)
		return (FAILURE);
	wordlen = ft_strlen_ns(*token_word);
	if (wordlen < 2)
		return (FAILURE);
	quote_removal_word = ft_substr(*token_word, 1, wordlen - 2);
	if (!quote_removal_word)
		return (perror_ret_int("malloc", FAILURE));
//	printf("before removal:%s\n", *token_word);
	free_1d_alloc(*token_word);
	*token_word = quote_removal_word;
	return (SUCCESS);
}




























