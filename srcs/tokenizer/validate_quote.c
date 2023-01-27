/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 10:09:06 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 10:13:19 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	validate_quote(t_list_bdi *tokenlist_head)
{
	t_list_bdi		*node;
	t_token_elem	*now_token;
	size_t			word_len;
	char 			*word;

	if (!tokenlist_head)
		return (FAILURE);
	node = tokenlist_head;
	while (node)
	{
		now_token = node->content;
		if (now_token->is_quoted)
		{
			printf("quoted token:%s\n", now_token->word);
			word = now_token->word;
			word_len = ft_strlen_ns(word);
			if (word_len < 2 || word[0] != word[word_len - 1])
			{
				printf("unclosed quoted token:%s\n", now_token->word);
				ft_dprintf(STDERR_FILENO, "minishell: unclosed quote `%c'\n", now_token->word[0]);
				return (FAILURE);
			}
		}
		node = node->next;
	}
	return (SUCCESS);
}
