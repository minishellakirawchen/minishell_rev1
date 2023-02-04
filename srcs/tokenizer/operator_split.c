/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 17:15:53 by takira            #+#    #+#             */
/*   Updated: 2023/02/04 17:42:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* FREE OK */
// TODO: Norminette
#include "tokenizer.h"

/*
 ["hello world"]=['good bye']=[|cat]->[Makefile>out]=['hoge']
                         keep^ ^^             ^^^   ^keep
                               split         split
*/
int	add_splitted_word_to_splitted_list(char *word, t_list_bdi **list_head, t_split_info *s_info);


t_split_info	*create_split_info(const char *src, const char *delim, const char *set)
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

void	handle_quoted(t_split_info *s_info)
{
	char	watching_chr;

	if (!s_info)
		return ;

	watching_chr = ft_strchr(s_info->setchars, s_info->src[s_info->head_idx + s_info->word_len])[0];
	s_info->quote_chr = watching_chr;
//			printf("\n$$ debug split in quote :: word:%s, quote:%c\n\n", &s_info->src[s_info->head_idx + s_info->word_len], quote_chr);
	s_info->is_quoted = true;
	s_info->word_len += 1;
	while (s_info->src[s_info->head_idx + s_info->word_len] && s_info->src[s_info->head_idx + s_info->word_len] != watching_chr)
		s_info->word_len += 1;
	if (s_info->src[s_info->head_idx + s_info->word_len])
		s_info->word_len += 1;
	if (s_info->src[s_info->head_idx + s_info->word_len] && !is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], STR_OPERATOR)) // echo "hoge";
		s_info->is_connect_to_next_word = true; //																		 ^^ connect false

}

void	get_split_params(t_split_info *s_info, const char *opes)
{
	char	watching_chr;

	if (!s_info || !opes)
		return ;
	if (is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], s_info->setchars))
		handle_quoted(s_info);
		// operation -> go to next to operation
	else if (is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], STR_SUBSHELL))
		s_info->word_len++;
	else if (is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], opes))
	{
		watching_chr = ft_strchr(opes, s_info->src[s_info->head_idx + s_info->word_len])[0];
		while (s_info->src[s_info->head_idx + s_info->word_len] && s_info->src[s_info->head_idx + s_info->word_len] == watching_chr)
			s_info->word_len++;
	}
	else
	{
		while (s_info->src[s_info->head_idx + s_info->word_len]
		&& !is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], opes)
		&& !is_chr_in_str(s_info->src[s_info->head_idx + s_info->word_len], s_info->setchars))
			s_info->word_len++;
	}
}

int	get_splitted_word_to_list(t_list_bdi **splitted_list_head, t_split_info *s_info)
{
	char		 	*splittd_word;

	if (!s_info || !splitted_list_head)
		return (FAILURE);

	splittd_word = ft_substr(s_info->src, s_info->head_idx, s_info->word_len);
	if (!splittd_word)
		return (perror_ret_int("malloc", FAILURE));
	if (add_splitted_word_to_splitted_list(splittd_word, splitted_list_head, s_info) == FAILURE)
	{
		free(splittd_word);
		return (FAILURE);
	}
	return (SUCCESS);
}

t_list_bdi	*get_split_before_after_opes(const char *src, const char *opes, char *quote)
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

int	add_splitted_word_to_splitted_list(char *word, t_list_bdi **list_head, t_split_info *s_info)
{
	t_token_elem	*token_elem;
	t_list_bdi		*new_list;

	if (!word || !list_head || !s_info)
		return (FAILURE);
	token_elem = create_token_elem(word, s_info->is_connect_to_next_word, s_info->is_quoted, s_info->quote_chr);
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
// どうやって間に入れれば良いんだ...?->dekita

int	split_by_operators(t_list_bdi **token_head)
{
	t_list_bdi		*now_node;
	t_list_bdi		*prev;
	t_list_bdi		*last_node;
	t_token_elem	*token_elem;
	t_list_bdi		*splitted_list_head;
	t_token_elem	*last_token;

	if (!token_head || !*token_head)
		return (FAILURE);
	now_node = *token_head;
	prev = NULL;
	while (now_node)
	{
		token_elem = now_node->content;
		if (!token_elem->is_quoted && is_str1chrs_in_str2(STR_OPERATOR, token_elem->word))
		{
			splitted_list_head = get_split_before_after_opes(token_elem->word, STR_OPERATOR, STR_SPACE);
			if (!splitted_list_head)
			{
				ft_lstclear_bdi(token_head, free_token_elem);
				return (FAILURE);
			}
			if (prev)
				prev->next = splitted_list_head;
			else
				*token_head = splitted_list_head;

			last_node = ft_lstlast_bdi(splitted_list_head);
			last_token = last_node->content;
			last_token->is_connect_to_next_word = token_elem->is_connect_to_next_word;

			last_node->next = now_node->next;
			ft_lstdelone_bdi(&now_node, free_token_elem);
			now_node = last_node;
		}
		prev = now_node;
		now_node = now_node->next;
	}
	return (SUCCESS);
}
























