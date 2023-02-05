/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 09:45:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:04:29 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static int	do_expand(t_list_bdi **popped_node, \
t_list_bdi **wildcard_save_list, t_list_bdi **expanded_tokens)
{
	t_token_elem	*token_elem;

	token_elem = (*popped_node)->content;
	if (get_wildcard_tokens(wildcard_save_list, \
	token_elem->word, token_elem->wildcard_valid_flag) == FAILURE)
	{
		ft_lstclear_bdi(wildcard_save_list, free);
		ft_lstdelone_bdi(popped_node, free_token_elem);
		ft_lstclear_bdi(expanded_tokens, free_token_elem);
		return (FAILURE);
	}
	if (ft_lstsize_bdi(*wildcard_save_list) != 0)
	{
		ft_lstdelone_bdi(popped_node, free_token_elem);
		*popped_node = *wildcard_save_list;
	}
	return (SUCCESS);
}

static bool	is_not_expandable_command(t_list_bdi *first_token)
{
	t_token_elem	*token_elem;

	if (!first_token)
		return (false);
	token_elem = first_token->content;
	return (is_same_str("export", token_elem->word));
}

/*
 if character * is wildcard, it is expanded to match string.
 if character "*" is string, it is not expanded.
 */
int	expanded_wildcard_to_token_list(t_list_bdi **token_list)
{
	t_list_bdi		*expanded_tokens;
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;
	t_list_bdi		*wildcard_match_tokens;

	if (!token_list)
		return (FAILURE);
	if (is_not_expandable_command(*token_list))
		return (SUCCESS);
	expanded_tokens = NULL;
	while (*token_list)
	{
		popped_node = ft_lstpop_bdi(&(*token_list));
		token_elem = popped_node->content;
		if (check_valid_wildcard_in_word(token_elem))
			if (do_expand(&popped_node, \
			&wildcard_match_tokens, &expanded_tokens) == FAILURE)
				return (FAILURE);
		ft_lstadd_back_bdi(&expanded_tokens, popped_node);
	}
	*token_list = expanded_tokens;
	return (SUCCESS);
}
