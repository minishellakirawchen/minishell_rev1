/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrange_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 17:07:57 by takira            #+#    #+#             */
/*   Updated: 2023/02/03 22:48:26 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* FREE OK */
#include "tokenizer.h"

//static void	delete_empty_elem(t_list_bdi **tokenlist_head);
static int	valid_control_operator(t_list_bdi **tokenlist_head);
static void	set_elem_type_if_operator(t_list_bdi **tokenlist_head);
static int	set_elem_type_if_word(t_list_bdi **tokenlist_head);
static void	set_parenthesis_no(t_list_bdi **tokenlist_head);

// TODO: $ < > outrfile
int	arrange_and_validate_token_list(t_list_bdi **tokenlist_head)
{
	if (!tokenlist_head || !*tokenlist_head)
		return (PROCESS_ERROR);
//	debug_print_tokens(*tokenlist_head, "before arrange");
	if (valid_control_operator(tokenlist_head) == FAILURE)
		return (SYNTAX_ERROR);
	set_elem_type_if_operator(tokenlist_head);
	if (validate_quote(*tokenlist_head) == FAILURE)
		return (SYNTAX_ERROR);
	if (validate_syntax_parenthesis_pairs(*tokenlist_head) == FAILURE)
		return (SYNTAX_ERROR);
	set_parenthesis_no(tokenlist_head);
	if (validate_syntax_operators(*tokenlist_head) == FAILURE)
		return (SYNTAX_ERROR);
//	debug_print_tokens(*tokenlist_head, "validated");
	set_elem_type_if_word(tokenlist_head);
	if (ft_lstsize_bdi(*tokenlist_head) == 0)
		return (PROCESS_ERROR);
//	debug_print_tokens(*tokenlist_head, "set word type");
	return (EXIT_SUCCESS);
}

/*
 ( () ( () () ) )
 0 11 1 22 22 1 0
*/
static void	set_parenthesis_no(t_list_bdi **tokenlist_head)
{
	t_list_bdi		*node;
	t_token_elem	*token;
	int				parentesis_no;

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
			token->subshell_depth = parentesis_no;
		}
		else if (token->type == e_subshell_end)
		{
			token->subshell_depth = parentesis_no;
			parentesis_no--;
		}
		else
			token->subshell_depth = parentesis_no;
		node = node->next;
	}
}

/*
 validate control sign
 error: <<<, ;;, |||, &&&, etc.
*/
static int	valid_control_operator(t_list_bdi **tokenlist_head)
{
	t_list_bdi			*node;
	t_token_elem	*token;

	node = *tokenlist_head;
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
	t_list_bdi		*node;
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
		if (token->is_connect_to_next && next_token && next_token->type != e_init)
			token->is_connect_to_next = false;
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
