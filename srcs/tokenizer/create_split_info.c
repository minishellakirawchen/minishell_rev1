/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_split_info.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 13:40:08 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:41:21 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

t_split_info	*create_split_info(\
const char *src, const char *delim, const char *set)
{
	t_split_info	*s_info;

	if (!src)
		return (NULL);
	s_info = (t_split_info *)malloc(sizeof(t_split_info));
	if (!s_info)
		return (perror_ret_nullptr("malloc"));
	s_info->head_idx = 0;
	s_info->word_len = 0;
	s_info->src = src;
	s_info->delim = delim;
	s_info->setchars = set;
	s_info->is_connect_to_next_word = false;
	s_info->is_quoted = false;
	s_info->quote_chr = '\0';
	return (s_info);
}

void	init_split_info(t_split_info *s_info)
{
	if (!s_info)
		return ;
	s_info->word_len = 0;
	s_info->quote_chr = '\0';
	s_info->is_quoted = false;
	s_info->is_connect_to_next_word = false;
}
