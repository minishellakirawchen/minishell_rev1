/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:28 by takira            #+#    #+#             */
/*   Updated: 2023/01/18 16:42:26 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

// ft_split_set(char **split, char delim, char *setchr)
// もしくは, char *input_lineを前からチェックして切り取り文字が出てきたらsubstrして構造体に入れていく？
// 一旦後者で実装してみる

/* if str1[i] in str2, return true */

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

int	tokenize_input_line(t_info *info, const char *readline_input)
{
	if (!info | !readline_input)
		return (FAILURE);
	// split by space
	//   char **input -> list
	info->tokenlist_head = get_space_splitted_tokenlist(readline_input,
														STR_SPACE, STR_QUOTE);
	debug_print_token_word(info->tokenlist_head, "token list");
	if (!info->tokenlist_head)
		return (FAILURE);
	// split by control operation and redirection
	//   prev->before_list->next  ->  prev->split1->split2...->next

	return (SUCCESS);
}
