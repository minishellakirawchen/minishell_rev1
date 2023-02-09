/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_splitted_word_token.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 19:17:13 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:18:28 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static char	*get_splitted_word(t_split_info *s_info)
{
	char	*word;

	s_info->is_quoted = false;
	while (s_info->src[s_info->head_idx] \
	&& is_chr_in_str(s_info->src[s_info->head_idx], s_info->delim))
		s_info->head_idx += 1;
	if (!s_info->src[s_info->head_idx])
		word = ft_strdup("");
	else
	{
		word = get_trimmed_word(s_info);
		s_info->head_idx += s_info->word_len;
	}
	if (!word)
		return (perror_ret_nullptr("malloc"));
	return (word);
}

t_token_elem	*create_splitted_word_token_from_src(t_split_info *s_info)
{
	t_token_elem	*new_token;
	char			*word;

	word = get_splitted_word(s_info);
	if (!word)
		return (perror_ret_nullptr("malloc"));
	new_token = create_token_elem(word, s_info->is_connect_to_next_word, \
	s_info->is_quoted, '\0');
	if (!new_token)
	{
		free_ret_nullprt(word);
		free_token_elem(new_token);
		return (perror_ret_nullptr("malloc"));
	}
	if (s_info->is_quoted && new_token->word)
		new_token->quote_chr = new_token->word[0];
	return (new_token);
}
