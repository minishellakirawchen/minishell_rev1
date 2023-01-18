/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   space_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:36:28 by takira            #+#    #+#             */
/*   Updated: 2023/01/18 17:04:10 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static t_split_info	init_split_info(const char *src, const char *delim, const char *setchars);
static t_token_elem	*create_word_token_from_src(t_split_info *s_info);
static char			*get_trimmed_word(const char *src, const char *delim, const char *setchars, size_t *len, bool *is_connect2next);

// !delim  -> return (src)
// !setchars -> just split by delim
// 切り取るwordのhead, sizeをget, listにappendする
t_list	*get_space_splitted_tokenlist(const char *src, const char *delim, const char *setchars)
{
	t_list			*tokenlist_head;
	t_list			*new_list;
	t_token_elem	*new_token;
	t_split_info	s_info;

	tokenlist_head = NULL;
	if (!src || is_str1chrs_in_str2(delim, setchars))
		return (NULL);
	s_info = init_split_info(src, delim, setchars);
	while (src[s_info.head_idx])
	{
		new_token = create_word_token_from_src(&s_info);
		new_list = ft_lstnew((void *)new_token);
		if (!new_token || !new_list)
		{
			ft_lstclear(&tokenlist_head, free_token_elem);
			return (perror_ret_nullptr("malloc"));
		}
		ft_lstadd_back(&tokenlist_head, new_list);
	}
	return (tokenlist_head);
}

static t_token_elem	*create_word_token_from_src(t_split_info *split)
{
	t_token_elem	*new_token;
	char			*word;

	while (split->src[split->head_idx] && is_chr_in_str(split->src[split->head_idx], split->delim))
		split->head_idx += 1;
	if (!split->src[split->head_idx])
		word = ft_strdup("");
	else
	{
		word = get_trimmed_word(&split->src[split->head_idx], split->delim, split->setchars, &split->word_len, &split->is_connect_to_next_word);
		split->head_idx += split->word_len;
	}
	new_token = (t_token_elem *)malloc(sizeof(t_token_elem));
	if (!word || !new_token)
	{
		word = free_1d_alloc(word);
		free_token_elem(new_token);
		return (perror_ret_nullptr("malloc"));
	}
	new_token->word = (char *)word;
	new_token->type = e_init;
	new_token->is_connect_to_next_word = split->is_connect_to_next_word;
	return (new_token);
}

// topがdelimでないsrcから、次のdelimまでを切り取る
static char *get_trimmed_word(const char *src, const char *delim, const char *setchars, size_t *len, bool *is_connect2next)
{
	char	*word;
	char	setchr;

	if (!src || !delim || !setchars)
		return (NULL);
	*is_connect2next = false;
	*len = 0;
	if (src[*len] && is_chr_in_str(src[*len], setchars))
	{
		setchr = ft_strchr(setchars, src[*len])[0];
		*len += 1;
		while (src[*len] && src[*len] != setchr)
			*len += 1;
		if (src[*len])
			*len += 1;
	}
	else
		while (src[*len] && !is_chr_in_str(src[*len], setchars) && !is_chr_in_str(src[*len], delim))
			*len += 1;
	if (src[*len] && !is_chr_in_str(src[*len], delim))
		*is_connect2next = true;
	word = ft_substr(src, 0, *len);
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
	split.setchr = 0;
	return (split);
}
