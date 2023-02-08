/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 21:20:42 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 16:14:00 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static void	print_parenthesis_error(int cnt)
{
	if (cnt > 0)
		ft_dprintf(STDERR_FILENO, \
		"minishell: syntax error unclosed parenthesis `('\n");
	else
		ft_dprintf(STDERR_FILENO, \
		"minishell: syntax error unclosed parenthesis `)'\n");
}

int	validate_syntax_parenthesis_pairs(t_list_bdi *tokenlist_head)
{
	t_list_bdi		*node;
	t_token_elem	*token;
	int				cnt;

	if (!tokenlist_head)
		return (FAILURE);
	node = tokenlist_head;
	cnt = 0;
	while (node)
	{
		token = node->content;
		if (token->type == e_subshell_start)
			cnt++;
		if (token->type == e_subshell_end)
			cnt--;
		node = node->next;
	}
	if (cnt == 0)
		return (SUCCESS);
	print_parenthesis_error(cnt);
	return (FAILURE);
}

bool	is_exception_empty_double_parenthesis(t_list_bdi **node)
{
	t_token_elem	*token;


	if (!node || ft_lstsize_bdi(*node) < 4)
		return (false);
	token = (*node)->content;
	if (!is_same_str(token->word, "("))
		return (false);
	token = (*node)->next->content;
	if (!is_same_str(token->word, "("))
		return (false);
	token = (*node)->next->next->content;
	if (!is_same_str(token->word, ")"))
		return (false);
	token = (*node)->next->next->content;
	if (!is_same_str(token->word, ")"))
		return (false);
	(*node) = (*node)->next->next->next->next;
	return (true);
}

/*
 check now and next token type
 OK echo hello; < infile
 OK echo hello; > outfile
*/
int	validate_syntax_operators(t_list_bdi *tokenlist_head)
{
	t_list_bdi		*node;
	t_token_elem	*token;
	t_token_elem	*next_token;
	bool			is_head;

	node = tokenlist_head;
	next_token = NULL;
	is_head = true;
	while (node)
	{
		token = node->content;
		if (node->next)
			next_token = node->next->content;
		else
			next_token = NULL;
		if (is_exception_empty_double_parenthesis(&node))
		{
			is_head = false;
			continue ;
		}
		if (validate_operator_tokens(token, next_token, is_head) == FAILURE)
			return (FAILURE);
		node = node->next;
		if (is_head)
			is_head = false;
	}
	return (SUCCESS);
}
