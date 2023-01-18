/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 17:15:53 by takira            #+#    #+#             */
/*   Updated: 2023/01/18 23:28:35 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

//echo hello && (cd /bin && pwd && ls) || cat <infile >outfile

// $a1
// a1="echo hello world | cat Makefile"
// "echo hello world | cat Makefile" <- echo command
// {"echo", "hello", "world", "|", "cat", "Makefile", NULL} -nも有効

// $a2
// a2="hoge       hello world"
// echo $a2 -> hoge hello world
// env      -> a2=echo       hello world

// expansionは全て区切ったあとに実施しなければ、引数との判別ができなくなる

// char *word = list->token_elem->word
// word = "cat<infile|ls>>out&&test;cat<file|grep"
// split before/after operator
//   "cat"
//   "<"
//   "infile"
//   "|"
//   "ls"
//   ">>"
//   "out"
//   "&&"
//   "test"
//   ";"
//   "cat"
//   "<"
//   "file"
//   "|"
//   "grep"

// quotedは既に分離済み
// 既に付与している結合フラグを壊したくない
// 難しいぞ？->そんなことないかも
// 結合フラグは文字列、qupte間のみでは？そんなことはない 面倒かも...
//
//
// ["hello world"]=['good bye']=[|cat]->[Makefile>out]=['hoge']
//                         keep^ ^^             ^^^   ^keep
//                               split         split
// connect_to_nextなので、last elemにフラグを立てる

t_list	*get_split_before_after_opes(const char *src, const char *opes, char *quote)
{
	size_t			head_idx;
	size_t			word_len;
	char		 	*splittd_word;
	char			watching_chr;
	t_token_elem	*token_elem;
	t_list			*new_list;
	t_list			*splitted_list_head;
	bool			is_connect_to_next;
	char			quote_chr;

	splitted_list_head = NULL;
	head_idx = 0;
	while (src[head_idx])
	{
		word_len = 0;
		quote_chr = '\0';
		is_connect_to_next = false;

		// quote -> go to next quote
//		printf("%s\n", &src[head_idx + word_len]);
		if (is_chr_in_str(src[head_idx + word_len], quote))
		{
			watching_chr = ft_strchr(quote, src[ + word_len])[0];
			quote_chr = watching_chr;
			word_len++;
			while (src[head_idx + word_len] && src[head_idx + word_len] != watching_chr)
				word_len++;
			if (src[head_idx + word_len])
				word_len++;
			if (src[head_idx + word_len])
				is_connect_to_next = true;
		}
		// operation -> go to next to operation
		else if (is_chr_in_str(src[head_idx + word_len], opes))
		{
			watching_chr = ft_strchr(opes, src[head_idx + word_len])[0];
			while (src[head_idx + word_len] && src[head_idx + word_len] == watching_chr)
				word_len++;
		}
		// just splittd_word -> go to next quote, opes or null
		else
		{
			while (src[head_idx + word_len] && !is_chr_in_str(src[head_idx + word_len], opes) && !is_chr_in_str(src[head_idx + word_len], quote))
				word_len++;
		}

		splittd_word = ft_substr(src, head_idx, word_len);
		token_elem = (t_token_elem *)malloc(sizeof(t_token_elem));
		if (!splittd_word || !token_elem)
		{
			splittd_word = free_1d_alloc(splittd_word);
			free_token_elem(token_elem);
			return (perror_ret_nullptr("malloc"));
		}
		token_elem->word = splittd_word;
		token_elem->type = e_init;
		token_elem->is_connect_to_next_word = is_connect_to_next;
		token_elem->quote_chr = quote_chr;
		new_list = ft_lstnew(token_elem);
		if (!new_list)
		{
			splittd_word = free_1d_alloc(splittd_word);
			free_token_elem(token_elem);
			return (perror_ret_nullptr("malloc"));
		}
		ft_lstadd_back(&splitted_list_head, new_list);

		head_idx += word_len;
	}
//	debug_print_token_word(splitted_list_head, "splitted ");
	return (splitted_list_head);
}


// quote


// while(now)
// {
//    // do something
//    now = now->next;
// }
//
// now  -----> next
//  v           ^
// new1 ->..-> new2
//
// どうやって間に入れれば良いんだ...?
int	split_by_operators(t_list **token_head)
{
	t_list			*now_node;
//	t_list			*prev_node;
//	t_list			*next_node;
//	t_list			*last_node;
	t_token_elem	*elem;
//	t_token_elem	*last_elem;
	t_list			*splitted_list;

	if (!token_head || !*token_head)
		return (FAILURE);
	now_node = *token_head;
	while (now_node)
	{
		elem = now_node->content;
		splitted_list = get_split_before_after_opes(elem->word, STR_OPERATOR, STR_QUOTE);
		debug_print_token_word(splitted_list, "split opes");
		/*
		if (!elem->is_quoted)
		{
			splitted_list = get_split_before_after_opes(elem->word, STR_OPERATOR, STR_SPACE);
			if (!splitted_list)
			{
				ft_lstclear(token_head, free_token_elem);
				return (FAILURE);
			}
			last_node = ft_lstlast(splitted_list);
			last_elem = last_node->content;
			if (elem->is_connect_to_next_word)
				last_elem->is_connect_to_next_word = true;

			next_node = now_node->next;
			last_node->next = next_node;
			ft_lstdelone(now_node, free_token_elem);
			now_node = splitted_list;
		}
		*/
		now_node = now_node->next;
	}
	return (SUCCESS);
}























