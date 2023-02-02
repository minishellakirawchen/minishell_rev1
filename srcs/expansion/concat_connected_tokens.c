/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_connected_tokens.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 10:36:47 by takira            #+#    #+#             */
/*   Updated: 2023/02/02 21:51:01 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/*
bool	check_valid_wildcard_in_word(t_list_bdi *token_node)
{
	size_t	idx;

	t_token_elem	*token_elem;
	if (!token_node)
		return (false);
	token_elem = token_node->content;
	if (!token_elem->wildcard_valid_list)
			return (is_expandable_wildcard_in_str(token_elem->word, token_elem->is_quoted));
	idx = 0;
	while (token_elem->word[idx])
	{
		if (token_elem->wildcard_valid_list[idx] == 1)
			return (false);
		idx++;
	}
	return (true);
}
*/

bool	check_valid_wildcard_in_word(t_token_elem *token)
{
	size_t	idx;

	if (!token)
		return (false);
//	if (!token->wildcard_valid_list)
//		return (is_expandable_wildcard_in_str(token->word, token->is_quoted));
	idx = 0;
	while (token->word[idx])
	{
		if (token->wildcard_valid_list[idx] == 1)
			return (true);
		idx++;
	}
	return (false);
}

int get_wildcard_valid_lsit(t_token_elem **token_elem)
{
	size_t	word_len ;
	size_t	idx;

	if (!token_elem || !*token_elem ||  !(*token_elem)->word)
		return (FAILURE);
	word_len = ft_strlen_ns((*token_elem)->word);
	(*token_elem)->wildcard_valid_list = (int *)ft_calloc(sizeof(int), word_len);
	if (!(*token_elem)->wildcard_valid_list)
		return (perror_ret_int("malloc", FAILURE));
	if ((*token_elem)->is_quoted)
		return (SUCCESS);
	idx = 0;
	while ((*token_elem)->word[idx])
	{
		if ((*token_elem)->word[idx] == '*')
			(*token_elem)->wildcard_valid_list[idx] = 1;
		idx++;
	}
	return (SUCCESS);
}

int	concat_wildcard_valid_list(t_token_elem **dst, t_token_elem *src)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	dst_idx;
	size_t	src_idx;
	int		*list;

	if (!dst || !*dst || !src)
		return (FAILURE);
	dst_len = ft_strlen_ns((*dst)->word);
	src_len = ft_strlen_ns(src->word);
	printf("dst:%s, src:%s\n", (*dst)->word, src->word);
	list = (int *)ft_calloc(sizeof(int), dst_len + src_len);
	dst_idx = 0;
	while (dst_idx < dst_len)
	{
		list[dst_idx] = (*dst)->wildcard_valid_list[dst_idx];
		dst_idx++;
	}
	free((*dst)->wildcard_valid_list);
	(*dst)->wildcard_valid_list = list;
	if (src->is_quoted)
		return (SUCCESS);
	src_idx = 0;
	while (src->word[src_idx])
	{
		if (src->word[src_idx] == '*')
			list[dst_idx + src_idx] = 1;
		src_idx++;
	}
	return (SUCCESS);
}

/* connect (*token_list)->content if "is_connect_to_next_word" */
int	concat_connected_tokens(t_list_bdi **token_list)
{
	t_list_bdi		*node;
	t_list_bdi		*popped_node;
	t_token_elem	*token;
	t_token_elem 	*append;

	if (!token_list)
		return (FAILURE);
	node = *token_list;
	while (node)
	{
		token = node->content;
		if (get_wildcard_valid_lsit(&token) == FAILURE)
			return (FAILURE);//TODO:free
//		token->is_wildcard_expandable = check_valid_wildcard_in_word(token);
		node = node->next;
		if (!token->is_connect_to_next_word)
			continue ;
		while (node)
		{
			popped_node = ft_lstpop_bdi(&node);
			append = popped_node->content;
			if (concat_wildcard_valid_list(&token, append) == FAILURE)
				return (FAILURE);//TODO:free
			token->word = concat_dst_to_src(&token->word, append->word);
			if (!token->word)
				return (perror_ret_int("malloc", FAILURE)); //TODO:free
			if (!append->is_connect_to_next_word)
				break ;
			ft_lstclear_bdi(&popped_node, free_token_elem);
		}
		ft_lstclear_bdi(&popped_node, free_token_elem);
		token->is_wildcard_expandable = check_valid_wildcard_in_word(token);
		token->is_connect_to_next_word = false;
	}
	return (SUCCESS);
}

/*
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
*/