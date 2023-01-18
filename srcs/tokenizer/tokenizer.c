/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:28 by takira            #+#    #+#             */
/*   Updated: 2023/01/18 15:32:56 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

// ft_split_set(char **split, char delim, char *setchr)
// もしくは, char *input_lineを前からチェックして切り取り文字が出てきたらsubstrして構造体に入れていく？
// 一旦後者で実装してみる

void	debug_print_token_word(t_list *head, char *str)
{
	t_list			*ptr = head;
	t_token_elem	*elem;
	if (str)
		ft_dprintf(STDERR_FILENO, "%s\n", str);
	ft_dprintf(STDERR_FILENO, " # ", str);
	while (ptr)
	{
		elem = ptr->content;
		ft_dprintf(STDERR_FILENO, "[%s]", elem->word);
		if (elem->is_connect_to_next_word && ptr->next)
			ft_dprintf(STDERR_FILENO, "=");
		if (!elem->is_connect_to_next_word && ptr->next)
			ft_dprintf(STDERR_FILENO, ",");
		ptr = ptr->next;
	}
	ft_dprintf(STDERR_FILENO, "\n\n");
}

/* if str1[i] in str2, return true */
bool	is_str1chrs_in_str2(const char *str1, const char *str2)
{
	size_t	idx;

	if (!str1 || !str2)
		return (false);
	idx = 0;
	while (str1[idx])
	{
		if (ft_strchr(str2, str1[idx]))
			return (true);
		idx++;
	}
	return (false);
}

// [   aaa bbb cccc | echo hello world "good &"morning && test;cat<Makefile>out]
//  ↓
// [aaa],[bbb],[cccc]		type:word
// [|],					 	type:operator
// [echo],[hello],[world]	type:word
// ["good &"morning]		type:word
// [&&]						type:operator
// [test]					type:word
// [;]						type:operator
// [cat]					type:word
// [<]						type:redirection
// [Makefile]				type:word
// [>]						type:redirection
// [out]					type:word

// topがdelimでないsrcから、次のdelimまでを切り取る
char *get_trimmed_word(const char *src, const char *delim, const char *setchars, size_t *len, bool *is_connect2next)
{
	char	*word;
	char	setchr;

	if (!src || !delim || !setchars)
		return (NULL);
	*is_connect2next = false;
	*len = 0;
	while (src[*len])
	{
//		printf("1 str:%s\n", &src[*len]);
		// delimを抜けるまで len++
//		while (src[*len] && is_chr_in_str(src[*len], delim)) // ...(1)
//			*len += 1;
		// "'の場合は次の"'まで移動
		if (src[*len] && is_chr_in_str(src[*len], setchars)) // ...(2)
		{
			setchr = ft_strchr(setchars, src[*len])[0];
//			printf(" 2-1 setchr:%c, str:%s\n", setchr, &src[*len]);
			// "'の次に移動
			*len += 1;
			// 次の"'まで移動
			while (src[*len] && src[*len] != setchr)
				*len += 1;
			// "'を抜ける, ここで!src[*len]だとunquoted error
			if (src[*len])
				*len += 1;
			//  close側"'の次が\0 or delimだったら、そこまでを切り取る
			if (!src[*len] || is_chr_in_str(src[*len], delim))
				break ;

			//  close側"'の次が"'だったら、connect2next=trueにして、そこまでを切り取る
			*is_connect2next = true;
			break ;
//		printf(" 2-2 setchr:%c, str:%s\n", setchr, &src[*len]);
		}
//		printf("3 str:%s\n", &src[*len]);

		// !delim, !setcharsの間 len++
		while (src[*len] && !is_chr_in_str(src[*len], setchars) && !is_chr_in_str(src[*len], delim))
			*len += 1;
		// !delim, !strchars を抜けた後にdelimであれば、そこまでを切り取る
		// そうでなければ先頭に戻る src[len]="'であれば、(2)から始まる
//		printf("4 str:%s\n", &src[*len]);

		if (!src[*len] || is_chr_in_str(src[*len], delim))
			break ;

		//  close側"'の次が"'だったら、connect2next=trueにして、そこまでを切り取る
		if (is_chr_in_str(src[*len], setchars))
		{
			*is_connect2next = true;
			break ;
		}
	}
	if (*len == 0)
		return (NULL);
	word = ft_substr(src, 0, *len);
	if (!word)
		return (perror_ret_nullptr("malloc"));
//	printf("5 word:[%s], len:%zu\n\n", word, *len);
	return (word);
}

t_split_info	init_split_info(const char *src, const char *delim, const char *setchars)
{
	t_split_info	s_info;

	s_info.src = src;
	s_info.delims = delim;
	if (!delim)
		s_info.delims = "";
	s_info.sets = setchars;
	if (!setchars)
		s_info.sets = "";
	s_info.is_connect_to_next_word = false;
	s_info.head_idx = 0;
	s_info.word_len = 0;
	return (s_info);
}

// !delim  -> return (src)
// !setchars -> just split by delim
// 切り取るwordのhead, sizeをget, listにappendする
t_list	*get_splitted_tokenlist(const char *src, const char *delim, const char *setchars)
{
	t_list			*tokenlist_head;
	t_list			*new_list;
	t_token_elem	*new_token;
	char 			*word;
//	t_split_info	s_info;
	size_t			word_len;
	size_t			head_idx;
	bool			is_connect_to_next;

//	init_split_info(src, delim, setchars);
	tokenlist_head = NULL;

	if (!src || is_str1chrs_in_str2(delim, setchars))
		return (NULL);
	head_idx = 0;
	while (src[head_idx])
	{
		// まずdelimを抜ける
		while (src[head_idx] && is_chr_in_str(src[head_idx], delim))
			head_idx++;
		if (!src[head_idx])
			break ;
		// src[head]から切り取る文字列を探してsubstrする
		word = get_trimmed_word(&src[head_idx], delim, setchars, &word_len, &is_connect_to_next);

		new_token = (t_token_elem *)malloc(sizeof(t_token_elem));
		if (!word || !new_token)
		{
			word = free_1d_alloc(word);
			free_token_elem(new_token);
			ft_lstclear(&tokenlist_head, free_token_elem);
			return (perror_ret_nullptr("malloc"));
		}
		new_token->word = word;
		new_token->type = e_init;
		new_token->is_connect_to_next_word = is_connect_to_next;
		new_list = ft_lstnew((void *)new_token);
		if (!new_list)
		{
			ft_lstclear(&tokenlist_head, free_token_elem);
			return (perror_ret_nullptr("malloc"));
		}
		ft_lstadd_back(&tokenlist_head, new_list);
		head_idx += word_len;
	}
	return (tokenlist_head);
}

int	tokenize_input_line(t_info *info, const char *readline_input)
{
	if (!info | !readline_input)
		return (FAILURE);
	// split by space
	//   char **input -> list
	info->tokenlist_head = get_splitted_tokenlist(readline_input, STR_SPACE, STR_QUOTE);
	debug_print_token_word(info->tokenlist_head, "token list");
	if (!info->tokenlist_head)
		return (FAILURE);
	// split by control operation and redirection
	//   prev->before_list->next  ->  prev->split1->split2...->next

	return (SUCCESS);
}