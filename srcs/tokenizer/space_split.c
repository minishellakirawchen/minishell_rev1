/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   space_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:36:28 by takira            #+#    #+#             */
/*   Updated: 2023/02/04 19:14:04 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* FREE OK */
#include "tokenizer.h"

static t_split_info	init_split_info(const char *src, const char *delim, const char *setchars);
static t_token_elem	*create_word_token_from_src(t_split_info *s_info);
static char *get_trimmed_word(t_split_info *s_info);

t_list_bdi	*get_new_splitted_token_node(t_split_info *s_info)
{
	t_list_bdi		*new_list;
	t_token_elem	*new_token;

	new_token = create_word_token_from_src(s_info);
	if (!new_token)
		return (perror_ret_nullptr("malloc"));
	new_list = ft_lstnew_bdi((void *)new_token);
	if (!new_list)
	{
		free_token_elem(new_token);
		return (perror_ret_nullptr("malloc"));
	}
	return (new_list);
}

t_list_bdi	*get_delim_splitted_tokenlist(const char *src, const char *delim, const char *setchars)
{
	t_list_bdi		*tokenlist_head;
	t_list_bdi		*new_list;
	t_split_info	s_info;

	tokenlist_head = NULL;
	if (!src || is_str1chrs_in_str2(delim, setchars))
		return (NULL);
	s_info = init_split_info(src, delim, setchars);
	while (src[s_info.head_idx])
	{
		new_list = get_new_splitted_token_node(&s_info);
		if (!new_list)
		{
			ft_lstclear_bdi(&tokenlist_head, free_token_elem);
			return (perror_ret_nullptr("malloc"));
		}
		ft_lstadd_back_bdi(&tokenlist_head, new_list);
	}
	return (tokenlist_head);
}

char *get_splitted_word(t_split_info *s_info)
{
	char			*word;

	s_info->is_quoted = false;
	while (s_info->src[s_info->head_idx] && is_chr_in_str(s_info->src[s_info->head_idx], s_info->delim))
		s_info->head_idx += 1;
	if (!s_info->src[s_info->head_idx])
		word = ft_strdup("");
	else
	{
		word = get_trimmed_word(s_info);
		s_info->head_idx += s_info->word_len;
	}
	if (!word)
		return(perror_ret_nullptr("malloc"));
	return (word);
}

/* explain here */
static t_token_elem	*create_word_token_from_src(t_split_info *s_info)
{
	t_token_elem	*new_token;
	char			*word;

	word = get_splitted_word(s_info);
	if (!word)
		return (perror_ret_nullptr("malloc"));
	new_token = create_token_elem(word, s_info->is_connect_to_next_word, s_info->is_quoted, '\0');
	if (!new_token)
	{
		free_1d_alloc(word);
		free_token_elem(new_token);
		return (perror_ret_nullptr("malloc"));
	}
	if (s_info->is_quoted && new_token->word)
		new_token->quote_chr = new_token->word[0];
	return (new_token);
}

/* cut src starting with not delim until next delim */
static char *get_trimmed_word(t_split_info *s_info)
{
	char	*word;
	char	setchr;

	if (!s_info->delim || !s_info->setchars)
		return (NULL);
	s_info->is_connect_to_next_word = false;
	s_info->word_len = 0;
	if (s_info->src[s_info->head_idx + s_info->word_len] && is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], s_info->setchars))
	{
		s_info->is_quoted= true;
		setchr = ft_strchr(s_info->setchars, s_info->src[s_info->head_idx + s_info->word_len])[0];
		s_info->word_len += 1;
		while (s_info->src[s_info->head_idx + s_info->word_len] && s_info->src[s_info->head_idx + s_info->word_len] != setchr)
			s_info->word_len += 1;
		if (s_info->src[s_info->head_idx + s_info->word_len])
			s_info->word_len += 1;
	}
	else
		while (s_info->src[s_info->head_idx + s_info->word_len] && !is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], s_info->setchars) && !is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], s_info->delim))
			s_info->word_len += 1;
	if (s_info->src[s_info->head_idx + s_info->word_len] && !is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], s_info->delim))
		s_info->is_connect_to_next_word = true;
	word = ft_substr(&s_info->src[s_info->head_idx], 0, s_info->word_len);
	if (!word)
		return (perror_ret_nullptr("malloc"));
	return (word);
}

static t_split_info	init_split_info(const char *src, const char *delim, const char *setchars)
{
	t_split_info	split;

	split.src = src;
	split.delim = delim;
	if (!delim)
		split.delim = "";
	split.setchars = setchars;
	if (!setchars)
		split.setchars = "";
	split.is_connect_to_next_word = false;
	split.head_idx = 0;
	split.word_len = 0;
	return (split);
}
