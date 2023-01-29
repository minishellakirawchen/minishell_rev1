/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrange_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 17:07:57 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 15:07:15 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

//static void	delete_empty_elem(t_list_bdi **tokenlist_head);
static int	valid_control_operator(t_list_bdi **tokenlist_head);
static void	set_elem_type_if_operator(t_list_bdi **tokenlist_head);
static int	set_elem_type_if_word(t_list_bdi **tokenlist_head);
static void	set_parenthesis_no(t_list_bdi **tokenlist_head);

// TODO: <> filename
int	arrange_and_validate_token_list(t_list_bdi **tokenlist_head)
{
	if (!tokenlist_head || !*tokenlist_head)
		return (FAILURE);

	debug_print_tokens(*tokenlist_head, "before arrange");

	if (valid_control_operator(tokenlist_head) == FAILURE)
		return (FAILURE);
	set_elem_type_if_operator(tokenlist_head);
	if (validate_quote(*tokenlist_head) == FAILURE)
		return (FAILURE);
	if (validate_syntax_parenthesis_pairs(*tokenlist_head) == FAILURE)
		return (FAILURE);

	set_parenthesis_no(tokenlist_head);

//	debug_print_tokens(*tokenlist_head, "set parenthesis no");

	debug_print_tokens(*tokenlist_head, "set opes");

//	delete_empty_elem(tokenlist_head);

//	debug_print_tokens(*tokenlist_head, "delete empty");

	if (validate_syntax_operators(*tokenlist_head) == FAILURE)
		return (FAILURE);

//	debug_print_tokens(*tokenlist_head, "validated");

	set_elem_type_if_word(tokenlist_head);

	if (ft_lstsize_bdi(*tokenlist_head) == 0)
		return (FAILURE);

	debug_print_tokens(*tokenlist_head, "set word type");

	return (SUCCESS);
}

//     ( () ( () () ) )
// no  0 11 2 33 44 2 0
// cnt 1 21 2 32 32 1 0

// ( -> no=no; cnt++, no++
// ) -> no=no; cnt--,


// () () ()
// 00 11 22
// 0  1  2  no

// ( () )
// 0 11 0   no

// ( () () )
// 0 11 22 0


//    ( () ( () () ) () )
//no  0 11 2 33 44 2 55 0
//    1 21 2 32 32 3 21 0  (+1, )-1
//(   0 1  2 3  4    5
//)      0    1  2 3  4 5

//closeは後ろから
//今までcloseしたnoを持っておく
//0番目は閉じていないが2,3は閉じて、次に4を閉じて最後に0みたいなことがある



// noを付与するときにcntもみる？

//minishell $> () (() ()) (())
//#input          :[() (() ()) (())]
//#arranged       :( 0 ,) 0 ,( 1 ,( 2 ,[)]) 1 ,[(]( 3 ,[)]) 2 ,[)]) 3 ,[(]( 4 ,[(]( 5 ,[)]) 4 ,[)]) 5


// TODO: 一旦これで妥協
// (の番号と同じ最も近い)を探せば相方が見つかる
// (の長さの配列を用意して、現在までみた(の数、とindexでcloseした)を管理すれば番号が付与できそう
// () () ()
// 00 00 00
//
// ( () ( () () ) )
// 0 11 1 22 22 1 0
static void	set_parenthesis_no(t_list_bdi **tokenlist_head)
{
	t_list_bdi			*node;
	t_token_elem	*token;
	ssize_t			parentesis_no;

	if (!tokenlist_head || !*tokenlist_head)
		return ;
	node = *tokenlist_head;
	parentesis_no = 0;
	while (node)
	{
		token = node->content;
		if (token->type == e_subshell_start)
		{
			parentesis_no++;
			token->depth = parentesis_no;
		}
		else if (token->type == e_subshell_end)
		{
			token->depth = parentesis_no;
			parentesis_no--;
		}
		else
			token->depth = parentesis_no;
		node = node->next;
	}
}


// TODO: separate set_elem_type_if_operator
static int	valid_control_operator(t_list_bdi **tokenlist_head)
{
	t_list_bdi			*node;
	t_token_elem	*token;

	node = *tokenlist_head;
	// validate control sign
	// error: <<<, ;;, |||, &&&, etc.
	while (node)
	{
		token = node->content;
		if (validate_operator_sign(token) == FAILURE)
			return (FAILURE);
		node = node->next;
	}
	return (SUCCESS);
}

static void	set_elem_type_if_operator(t_list_bdi **tokenlist_head)
{
	const char		*operators[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", NULL};
	size_t			idx;
	t_list_bdi			*node;
	t_token_elem	*token;

	if (!tokenlist_head || !*tokenlist_head)
		return ;
	node = *tokenlist_head;
	while (node)
	{
		token = node->content;
		if (!token->is_quoted && is_str1chrs_in_str2(STR_OPERATOR, token->word))
		{
			idx = 0;
			while (operators[idx] && !is_same_str(operators[idx], token->word))
				idx++;
			token->type = idx;
		}
		node = node->next;
	}
}

// echo hello > "hoge"huga
static int	set_elem_type_if_word(t_list_bdi **tokenlist_head)
{
	t_list_bdi			*node;
	t_token_elem	*token;
	t_token_elem	*next_token;

	if (!tokenlist_head || !*tokenlist_head)
		return (FAILURE);

	node = *tokenlist_head;
	while (node)
	{
		token = node->content;
		if (node->next)
			next_token = node->next->content;

		// disconnect to control operator
		if (token->is_connect_to_next_word && next_token && next_token->type != e_init)
			token->is_connect_to_next_word = false;
		if (next_token && is_tokentype_redirection(token->type))
		{
			if (next_token->type != e_init)
				return (FAILURE);
			if (token->type == e_redirect_in || token->type == e_redirect_out || token->type == e_redirect_append)
				next_token->type = e_file;
			if (token->type == e_heredoc)
				next_token->type = e_heredoc_eof;
		}
		if (token->type == e_init)
			token->type = e_word;
		node = node->next;
	}
	return (SUCCESS);
}

/*
static void	delete_empty_elem(t_list_bdi **tokenlist_head)
{
	t_list_bdi			*node;
	t_list_bdi			*prev;
	t_token_elem	*token;
	bool			del_this_node;

	if (!tokenlist_head || !*tokenlist_head)
		return ;
	node = *tokenlist_head;
	prev = NULL;
	while (node)
	{
		del_this_node = false;
		token = node->content;
		if (token->is_quoted && ft_strlen_ns(token->word) == 2)
			del_this_node = true;
		if (ft_strlen_ns(token->word) == 0)
			del_this_node = true;
		if (!del_this_node)
		{
			prev = node;
			node = node->next;
			continue ;
		}
		if (prev)
			prev->next = node->next;
		else
			*tokenlist_head = node->next;
		ft_lstdelone_bdi(&node, free_token_elem);
		if (prev)
			node = prev->next;
		else
			node = *tokenlist_head;

	}
}
*/