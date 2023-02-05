/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_last_splitted_node.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 13:46:48 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:51:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static void	handle_quoted(t_split_info *s)
{
	char	watching_chr;

	if (!s)
		return ;
	watching_chr = ft_strchr(\
	s->setchars, s->src[s->head_idx + s->word_len])[0];
	s->quote_chr = watching_chr;
	s->is_quoted = true;
	s->word_len += 1;
	while (s->src[s->head_idx + s->word_len] \
	&& s->src[s->head_idx + s->word_len] != watching_chr)
		s->word_len += 1;
	if (s->src[s->head_idx + s->word_len])
		s->word_len += 1;
	if (s->src[s->head_idx + s->word_len] \
	&& !is_chr_in_str(s->src[s->head_idx + s->word_len], STR_OPERATOR))
		s->is_connect_to_next_word = true;
}

static void	get_split_params(t_split_info *s, const char *opes)
{
	char	watching_chr;

	if (!s || !opes)
		return ;
	if (is_chr_in_str(s->src[s->head_idx + s->word_len], s->setchars))
		handle_quoted(s);
	else if (is_chr_in_str(s->src[s->head_idx + s->word_len], STR_SUBSHELL))
		s->word_len++;
	else if (is_chr_in_str(s->src[s->head_idx + s->word_len], opes))
	{
		watching_chr = ft_strchr(opes, s->src[s->head_idx + s->word_len])[0];
		while (s->src[s->head_idx + s->word_len] \
		&& s->src[s->head_idx + s->word_len] == watching_chr)
			s->word_len++;
	}
	else
	{
		while (s->src[s->head_idx + s->word_len] \
		&& !is_chr_in_str(s->src[s->head_idx + s->word_len], opes) \
		&& !is_chr_in_str(s->src[s->head_idx + s->word_len], s->setchars))
			s->word_len++;
	}
}

static int	add_splitted_word_to_splitted_list(\
char *word, t_list_bdi **list_head, t_split_info *s)
{
	t_token_elem	*token_elem;
	t_list_bdi		*new_list;

	if (!word || !list_head || !s)
		return (FAILURE);
	token_elem = create_token_elem(word, s->is_connect_to_next_word, \
	s->is_quoted, s->quote_chr);
	if (!token_elem)
		return (perror_ret_int("malloc", FAILURE));
	new_list = ft_lstnew_bdi(token_elem);
	if (!new_list)
	{
		free_token_elem(token_elem);
		return (perror_ret_int("malloc", FAILURE));
	}
	ft_lstadd_back_bdi(list_head, new_list);
	return (SUCCESS);
}

static int	get_splitted_word_to_list(\
t_list_bdi **splitted_list_head, t_split_info *s)
{
	char	*splittd_word;

	if (!s || !splitted_list_head)
		return (FAILURE);
	splittd_word = ft_substr(s->src, s->head_idx, s->word_len);
	if (!splittd_word)
		return (perror_ret_int("malloc", FAILURE));
	if (add_splitted_word_to_splitted_list(\
	splittd_word, splitted_list_head, s) == FAILURE)
	{
		free(splittd_word);
		return (FAILURE);
	}
	return (SUCCESS);
}

t_list_bdi	*get_split_before_after_opes(\
const char *src, const char *opes, char *quote)
{
	t_list_bdi		*splitted_list_head;
	t_split_info	*s_info;

	splitted_list_head = NULL;
	s_info = create_split_info(src, NULL, quote);
	if (!s_info)
		return (FAILURE);
	while (src[s_info->head_idx])
	{
		init_split_info(s_info);
		get_split_params(s_info, opes);
		if (get_splitted_word_to_list(&splitted_list_head, s_info) == FAILURE)
		{
			ft_lstclear_bdi(&splitted_list_head, free_token_elem);
			free(s_info);
			return (perror_ret_nullptr("malloc"));
		}
		s_info->head_idx += s_info->word_len;
	}
	free(s_info);
	return (splitted_list_head);
}
