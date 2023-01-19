/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 17:03:34 by takira            #+#    #+#             */
/*   Updated: 2023/01/19 17:10:50 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
// OK echo hello; < infile
// OK echo hello; > outfile
// validate relationship now and next

// [|,||,&&] + [<,<<,>,>>,(,init,]
// TODO: bash-3.2$ <>out
int	validate_syntax(t_token_elem *now_token, t_token_elem *next_token, bool is_head)
{
	t_token_type	type;
	t_token_type	next_type;
	bool			is_err;

	if (!now_token)
		return (FAILURE);
	type = now_token->type;
	next_type = e_nothing;
	if (next_token)
		next_type = next_token->type;
	is_err = false;
//	printf("word:%s\n", now_token->word);
	if (is_tokentype_semicolon(type))
		if (is_head || is_tokentype_pipe_or_and(next_type) || next_type == e_subshell_end)
			is_err = true;
	if (!is_err && is_tokentype_pipe_or_and(type))
		if (is_head || is_tokentype_semicolon(next_type) ||
			is_tokentype_pipe_or_and(next_type) || next_type == e_subshell_end || next_type == e_nothing)
			is_err = true;
	if (!is_err && type == e_subshell_start)
		if (is_tokentype_semicolon(next_type) ||
			is_tokentype_pipe_or_and(next_type) || next_type == e_subshell_end || next_type == e_nothing)
			is_err = true;
	if (!is_err && type == e_subshell_end)
		if (is_head || is_tokentype_subshell(next_type) || next_type == e_init)
			is_err = true;
	if (!is_err && is_tokentype_redirection(type))
		if (is_tokentype_semicolon(next_type) ||
			is_tokentype_pipe_or_and(next_type) ||
			is_tokentype_redirection(next_type) || is_tokentype_subshell(next_type) || next_type == e_nothing)
			is_err = true;
	if (!is_err && type == e_init)
		if (next_type == e_subshell_start)
			is_err = true;

	if (!is_err)
		return (SUCCESS);
	if (next_type == e_nothing)
		ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `newline'\n");
	else
		ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `%s'\n", next_token->word);
	return (FAILURE);
}
