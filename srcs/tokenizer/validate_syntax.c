/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 21:20:42 by takira            #+#    #+#             */
/*   Updated: 2023/01/21 14:53:33 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 17:03:34 by takira            #+#    #+#             */
/*   Updated: 2023/01/19 21:08:11 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static int	validate_context(t_token_elem *now_token, t_token_elem *next_token, bool is_head);

int validate_syntax_parenthesis_pairs(t_list *tokenlist_head)
{
	t_list			*node;
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
	if (cnt > 0)
		ft_dprintf(STDERR_FILENO, "minishell: unclosed parenthesis `('\n");
	else
		ft_dprintf(STDERR_FILENO, "minishell: unclosed parenthesis `)'\n");
	return (FAILURE);
}

// OK echo hello; < infile
// OK echo hello; > outfile
// validate relationship now and next
int validate_syntax_operators(t_list *tokenlist_head)
{
	t_list			*node;
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
		if (validate_context(token, next_token, is_head) == FAILURE)
			return (FAILURE);
		node = node->next;
		if (is_head)
			is_head = false;
	}
	return (SUCCESS);
}

// [|,||,&&] + [<,<<,>,>>,(,init,]
// TODO: bash-3.2$ <>out
// TODO: use BNF
static int	validate_context(t_token_elem *now_token, t_token_elem *next_token, bool is_head)
{
	t_token_type	type;
	t_token_type	next_type;
	bool			is_syntax_err;

	if (!now_token)
		return (FAILURE);
	type = now_token->type;
	next_type = e_nothing;
	if (next_token)
		next_type = next_token->type;
	is_syntax_err = false;
//	printf("word:%s\n", now_token->word);
	if (is_tokentype_semicolon(type))
		if (is_head || is_tokentype_list_operator(next_type) || next_type == e_subshell_end)
			is_syntax_err = true;
	if (!is_syntax_err && is_tokentype_pipe_or_and(type))
		if (is_head || is_tokentype_semicolon(next_type) ||
			is_tokentype_pipe_or_and(next_type) || next_type == e_subshell_end || next_type == e_nothing)
			is_syntax_err = true;
	if (!is_syntax_err && type == e_subshell_start)
		if (is_tokentype_semicolon(next_type) ||
			is_tokentype_pipe_or_and(next_type) || next_type == e_subshell_end || next_type == e_nothing)
			is_syntax_err = true;
	if (!is_syntax_err && type == e_subshell_end)
		if (is_head || next_type == e_subshell_start || next_type == e_init)
			is_syntax_err = true;
	if (!is_syntax_err && is_tokentype_redirection(type))
		if (is_tokentype_semicolon(next_type) ||
			is_tokentype_pipe_or_and(next_type) ||
			is_tokentype_redirection(next_type) || is_tokentype_subshell(next_type) || next_type == e_nothing)
			is_syntax_err = true;
	if (!is_syntax_err && type == e_init)
		if (next_type == e_subshell_start)
			is_syntax_err = true;

	if (!is_syntax_err)
		return (SUCCESS);
	if (next_type == e_nothing)
		ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `newline'\n");
	else
		ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `%s'\n", next_token->word);
	return (FAILURE);
}
