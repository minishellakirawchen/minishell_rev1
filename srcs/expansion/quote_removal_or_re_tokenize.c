/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal_or_re_tokenize.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:00:58 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 22:59:59 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static int	remove_quotes_in_word(char **token_word)
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
	free_1d_alloc(*token_word);
	*token_word = quote_removal_word;
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
			if (remove_quotes_in_word(&token_elem->word) == FAILURE)
				return (FAILURE);
		list_node = list_node->next;
	}
	return (SUCCESS);
}

static int	re_tokenize(t_list_bdi **expanded_tok_lst, t_list_bdi *popped_node)
{
	t_token_elem	*splitted_tok;
	t_list_bdi		*space_split_lst;
	t_token_elem	*token_elem;

	if (!expanded_tok_lst || !popped_node || !popped_node->content)
		return (FAILURE);
	token_elem = popped_node->content;
	if (!is_str1chrs_in_str2(STR_SPACE, token_elem->word))
		ft_lstadd_back_bdi(expanded_tok_lst, popped_node);
	else
	{
		space_split_lst = get_delim_splitted_tokenlist(\
		token_elem->word, STR_SPACE, STR_QUOTE);
		if (!space_split_lst)
		{
			ft_lstdelone_bdi(&popped_node, free_token_elem);
			return (FAILURE);
		}
		splitted_tok = space_split_lst->content;
		splitted_tok->is_connect_to_next = token_elem->is_connect_to_next;
		ft_lstadd_back_bdi(expanded_tok_lst, space_split_lst);
		ft_lstdelone_bdi(&popped_node, free_token_elem);
	}
	return (SUCCESS);
}

int	re_tokenize_tokens(t_list_bdi **src_tokens)
{
	t_list_bdi		*popped_node;
	t_list_bdi		*expanded_token_list;
	t_token_elem	*token_elem;

	if (!src_tokens)
		return (FAILURE);
	if (is_not_expandable_command(*src_tokens))
		return (SUCCESS);
	expanded_token_list = NULL;
	while (*src_tokens)
	{
		popped_node = ft_lstpop_bdi(src_tokens);
		token_elem = popped_node->content;
		if (token_elem->is_quoted)
		{
			ft_lstadd_back_bdi(&expanded_token_list, popped_node);
			continue ;
		}
		if (re_tokenize(&expanded_token_list, popped_node) == FAILURE)
			return (FAILURE);
	}
	*src_tokens = expanded_token_list;
	return (SUCCESS);
}

int	remove_quote_or_re_tokenize_tokens(t_list_bdi **src_tokens)
{
	t_list_bdi		*popped_node;
	t_list_bdi		*expanded_token_list;
	t_token_elem	*token_elem;

	if (!src_tokens)
		return (FAILURE);
	expanded_token_list = NULL;
	while (*src_tokens)
	{
		popped_node = ft_lstpop_bdi(src_tokens);
		token_elem = popped_node->content;
		if (token_elem->is_quoted)
		{
			if (remove_quotes_in_word(&token_elem->word) == FAILURE)
				return (FAILURE);
			ft_lstadd_back_bdi(&expanded_token_list, popped_node);
			continue ;
		}
		if (re_tokenize(&expanded_token_list, popped_node) == FAILURE)
			return (FAILURE);
	}
	*src_tokens = expanded_token_list;
	return (SUCCESS);
}
