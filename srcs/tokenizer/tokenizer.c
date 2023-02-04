/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:28 by takira            #+#    #+#             */
/*   Updated: 2023/02/04 21:48:16 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/* if str1[i] in str2, return true */

/*
 [   aaa bbb cccc | echo hello world "good &"morning && test;cat<Makefile>out]
  ↓
 [aaa],[bbb],[cccc]		type:word
 [|],				 	type:operator
 [echo],[hello],[world]	type:word
 ["good &"morning]		type:word
 [&&]					type:operator
 [test]					type:word
 [;]					type:operator
 [cat]					type:word
 [<]					type:redirection
 [Makefile]				type:word
 [>]					type:redirection
 [out]					type:word

 add tokentype
 disconnect word and operator ["hoge"]=[|]=[<] -> ["hoge"],[|],[<]
*/
int	tokenize_input_line(t_info *info, const char *readline_input)
{
	char	*space_trimmed_input;

	if (!info | !readline_input)
		return (PROCESS_ERROR);
	/* split by space */
	errno = 0;
	space_trimmed_input = ft_strtrim(readline_input, STR_SPACE);
	if (!space_trimmed_input)
		return (perror_ret_int("malloc", PROCESS_ERROR));

	/*   char **input -> list */
	info->tokenlist_head = get_delim_splitted_tokenlist(space_trimmed_input, STR_SPACE, STR_QUOTE);
	free_1d_alloc(space_trimmed_input);
	if (!info->tokenlist_head)
		return (PROCESS_ERROR);

//	debug_print_tokens(info->tokenlist_head, "split space");

	/* split by control operation and redirection */
	/* prev->before_list->next  ->  prev->split1->split2...->next */
	if (split_by_operators(&info->tokenlist_head) == PROCESS_ERROR)
		return (PROCESS_ERROR);

//	debug_print_tokens(info->tokenlist_head, "split opes");
	return (EXIT_SUCCESS);
}
