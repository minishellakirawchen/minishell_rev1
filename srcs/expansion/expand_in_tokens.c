/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 23:15:59 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 15:18:37 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static void	update_prev_connect_flag(t_list_bdi **save, t_list_bdi *popped)
{
	t_list_bdi		*prev_node;
	t_token_elem	*prev_token;
	t_token_elem	*popped_token;

	if (!save || !popped)
		return ;
	popped_token = popped->content;
	prev_node = ft_lstlast_bdi(*save);
	if (prev_node)
	{
		prev_token = prev_node->content;
		if (!popped_token->is_connect_to_next && prev_token->is_connect_to_next)
			prev_token->is_connect_to_next = false;
	}
}

static int	expand_or_delete_if_zerosize(\
t_token_elem *token_elem, t_list_bdi **save, t_info *info, t_list_bdi *popped)
{
	if (is_expandable_var(token_elem->word, token_elem->quote_chr))
	{
		if (expand_var_in_str(&token_elem->word, info) == FAILURE)
		{
			ft_lstclear_bdi(save, free_token_elem);
			return (FAILURE);
		}
		if (!token_elem->is_quoted && ft_strlen_ns(token_elem->word) == 0)
		{
			update_prev_connect_flag(save, popped);
			ft_lstdelone_bdi(&popped, free_token_elem);
			return (CONTINUE);
		}
	}
	return (SUCCESS);
}

int	expand_var_in_token_word(t_list_bdi **src_tokens, t_info *info)
{
	t_list_bdi		*node;
	t_list_bdi		*save;
	t_list_bdi		*popped;
	t_token_elem	*token_elem;
	int				exit_val;

	if (!src_tokens || !info)
		return (FAILURE);
	save = NULL;
	node = *src_tokens;
	while (node)
	{
		popped = ft_lstpop_bdi(&node);
		token_elem = popped->content;
		exit_val = expand_or_delete_if_zerosize(token_elem, &save, info, popped);
		if (exit_val == FAILURE)
			return (FAILURE);
		if (exit_val == CONTINUE)
			continue ;
		ft_lstadd_back_bdi(&save, popped);
	}
	*src_tokens = save;
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
	while (list_head)
	{
		token_elem = list_head->content;
		ft_strlcat(concat_str, token_elem->word, size + 1);
		list_head = list_head->next;
	}
	return (concat_str);
}
