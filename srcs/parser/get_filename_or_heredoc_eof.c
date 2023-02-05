/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redirect_list_helpter.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 08:59:50 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 09:15:00 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "parser.h"

static t_list_bdi	*get_expanded_tokenlist(
		t_list_bdi **token_get_from, bool *quoted, bool is_expand, t_info *info)
{
	t_list_bdi		*popped_token_node;
	t_token_elem	*tok_elem;
	t_list_bdi		*token_list;

	*quoted = false;
	token_list = NULL;
	while (*token_get_from)
	{
		popped_token_node = ft_lstpop_bdi(token_get_from);
		tok_elem = popped_token_node->content;
		if (is_expand && is_expandable_var(tok_elem->word, tok_elem->quote_chr))
		{
			if (expand_var_in_str(&tok_elem->word, info) == FAILURE)
			{
				ft_lstdelone_bdi(&popped_token_node, free_token_elem);
				ft_lstclear_bdi(&token_list, free_token_elem);
				return (FAILURE);
			}
		}
		ft_lstadd_back_bdi(&token_list, popped_token_node);
		*quoted |= tok_elem->is_quoted;
		if (!tok_elem->is_connect_to_next)
			break ;
	}
	return (token_list);
}

/* token node filename or heredoc eof is cleared in function */
char	*get_filename_or_heredoc_eof(
		t_list_bdi **token_get_frm, bool *quoted, bool is_expand, t_info *info)
{
	char			*str_concatted_token;
	t_list_bdi		*token_list;

	if (!token_get_frm || !*token_get_frm)
		return (NULL);
	token_list = get_expanded_tokenlist(token_get_frm, quoted, is_expand, info);
	if (!token_list)
		return (NULL);
	if (remove_quote_in_tokens(&token_list) == FAILURE)
	{
		ft_lstclear_bdi(&token_list, free_token_elem);
		return (NULL);
	}
	str_concatted_token = create_string_by_concat_tokens(token_list);
	if (!str_concatted_token)
	{
		ft_lstclear_bdi(&token_list, free_token_elem);
		return (NULL);
	}
	ft_lstclear_bdi(&token_list, free_token_elem);
	return (str_concatted_token);
}
