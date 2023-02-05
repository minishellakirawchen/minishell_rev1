/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:28 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:16:40 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

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
*/
int	tokenize_input_line(t_info *info, const char *readline_input)
{
	char	*space_trimmed_input;

	if (!info | !readline_input)
		return (PROCESS_ERROR);
	errno = 0;
	space_trimmed_input = ft_strtrim(readline_input, STR_SPACE);
	if (!space_trimmed_input)
		return (perror_ret_int("malloc", PROCESS_ERROR));
	info->tokenlist_head = get_delim_splitted_tokenlist(\
	space_trimmed_input, STR_SPACE, STR_QUOTE);
	free_1d_alloc(space_trimmed_input);
	if (!info->tokenlist_head)
		return (PROCESS_ERROR);
	if (split_by_operators(&info->tokenlist_head) == PROCESS_ERROR)
		return (PROCESS_ERROR);
	return (EXIT_SUCCESS);
}
