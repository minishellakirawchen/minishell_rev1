/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_trimmed_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 19:20:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/04 19:47:27 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static bool	is_now_reference_setchr(t_split_info *s_info);
static bool is_now_reference_not_delim(t_split_info *s_info);
static bool is_now_reference_not_delim_and_not_setchars(t_split_info *s_info);
static void	skip_inside_setchars(t_split_info *s_info);

/* cut src starting with not delim until next delim */
char *get_trimmed_word(t_split_info *s_info)
{
	char	*word;

	if (!s_info->delim || !s_info->setchars)
		return (NULL);
	s_info->is_connect_to_next_word = false;
	s_info->word_len = 0;
	if (is_now_reference_setchr(s_info))
		skip_inside_setchars(s_info);
	else
		while (is_now_reference_not_delim_and_not_setchars(s_info))
			s_info->word_len += 1;
	if (is_now_reference_not_delim(s_info))
		s_info->is_connect_to_next_word = true;
	word = ft_substr(&s_info->src[s_info->head_idx], 0, s_info->word_len);
	if (!word)
		return (perror_ret_nullptr("malloc"));
	return (word);
}

static bool is_now_reference_not_delim_and_not_setchars(t_split_info *s_info)
{
	if (!s_info)
		return (false);
	return (s_info->src[s_info->head_idx + s_info->word_len]
			&& !is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], s_info->setchars)
			&& !is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], s_info->delim));
}

static bool	is_now_reference_setchr(t_split_info *s_info)
{
	if (!s_info)
		return (false);
	return (s_info->src[s_info->head_idx + s_info->word_len]
	&& is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], s_info->setchars));
}

static bool is_now_reference_not_delim(t_split_info *s_info)
{
	if (!s_info)
		return (false);
	return (s_info->src[s_info->head_idx + s_info->word_len]
	&& !is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], s_info->delim));
}

static void	skip_inside_setchars(t_split_info *s_info)
{
	char setchr;

	if (!s_info)
		return ;
	s_info->is_quoted= true;
	setchr = ft_strchr(s_info->setchars, s_info->src[s_info->head_idx + s_info->word_len])[0];
	s_info->word_len += 1;
	while (s_info->src[s_info->head_idx + s_info->word_len] && s_info->src[s_info->head_idx + s_info->word_len] != setchr)
		s_info->word_len += 1;
	if (s_info->src[s_info->head_idx + s_info->word_len])
		s_info->word_len += 1;
}
