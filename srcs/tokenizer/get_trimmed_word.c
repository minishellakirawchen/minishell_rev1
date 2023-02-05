/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_trimmed_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 19:20:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:57:26 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static bool	is_now_reference_not_delim_and_not_setchars(t_split_info *s)
{
	if (!s)
		return (false);
	return (s->src[s->head_idx + s->word_len] \
			&& !is_chr_in_str(s->src[s->head_idx + s->word_len], s->setchars) \
			&& !is_chr_in_str(s->src[s->head_idx + s->word_len], s->delim));
}

static bool	is_now_reference_setchr(t_split_info *s)
{
	if (!s)
		return (false);
	return (s->src[s->head_idx + s->word_len] \
	&& is_chr_in_str(s->src[s->head_idx + s->word_len], s->setchars));
}

static bool	is_now_reference_not_delim(t_split_info *s)
{
	if (!s)
		return (false);
	return (s->src[s->head_idx + s->word_len] \
	&& !is_chr_in_str(s->src[s->head_idx + s->word_len], s->delim));
}

static void	skip_inside_setchars(t_split_info *s)
{
	char	setchr;

	if (!s)
		return ;
	s->is_quoted = true;
	setchr = ft_strchr(s->setchars, s->src[s->head_idx + s->word_len])[0];
	s->word_len += 1;
	while (s->src[s->head_idx + s->word_len] \
	&& s->src[s->head_idx + s->word_len] != setchr)
		s->word_len += 1;
	if (s->src[s->head_idx + s->word_len])
		s->word_len += 1;
}

/* cut src starting with not delim until next delim */
char	*get_trimmed_word(t_split_info *s)
{
	char	*word;

	if (!s->delim || !s->setchars)
		return (NULL);
	s->is_connect_to_next_word = false;
	s->word_len = 0;
	if (is_now_reference_setchr(s))
		skip_inside_setchars(s);
	else
		while (is_now_reference_not_delim_and_not_setchars(s))
			s->word_len += 1;
	if (is_now_reference_not_delim(s))
		s->is_connect_to_next_word = true;
	word = ft_substr(&s->src[s->head_idx], 0, s->word_len);
	if (!word)
		return (perror_ret_nullptr("malloc"));
	return (word);
}
