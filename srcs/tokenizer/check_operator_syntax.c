/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_operator_syntax.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 08:44:39 by takira            #+#    #+#             */
/*   Updated: 2023/02/09 13:54:40 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static void	validate_operator_list_token(\
t_token_type type, t_token_type next_type, bool *is_error, bool is_head)
{
	if (!is_tokentype_operator(type) || *is_error)
		return ;
	if (is_head)
	{
		*is_error = true;
		return ;
	}
	if (is_tokentype_semicolon(type))
		if (is_tokentype_semicolon(next_type) \
		|| is_tokentype_pipe_and_or(next_type))
			*is_error = true;
	if (type == e_ope_and || type == e_ope_or)
		if (is_tokentype_semicolon(next_type) \
		|| is_tokentype_pipe_and_or(next_type) \
		|| next_type == e_subshell_end \
		|| next_type == e_null)
			*is_error = true;
}

static void	validate_pipeline_commands_token(\
t_token_type type, t_token_type next_type, bool *is_error, bool is_head)
{
	if (*is_error)
		return ;
	if (type == e_ope_pipe)
	{
		if (is_head \
		|| is_tokentype_semicolon(next_type) \
		|| is_tokentype_pipe_and_or(next_type) \
		|| next_type == e_subshell_end \
		|| next_type == e_null)
			*is_error = true;
	}
	if (is_tokentype_redirection(type))
		if (is_tokentype_semicolon(next_type) \
		|| is_tokentype_pipe_and_or(next_type) \
		|| is_tokentype_redirection(next_type) \
		|| is_tokentype_subshell(next_type) \
		|| next_type == e_null)
			*is_error = true;
}

static void	validate_subshell_token(\
t_token_type type, t_token_type next_type, bool *is_error, bool is_head)
{
	if (*is_error)
		return ;
	if (type == e_subshell_start)
	{
		if (is_tokentype_semicolon(next_type) \
		|| is_tokentype_pipe_and_or(next_type) \
		|| next_type == e_subshell_end \
		|| next_type == e_null)
			*is_error = true;
	}
	if (type == e_subshell_end)
		if (is_head \
		|| next_type == e_subshell_start \
		|| next_type == e_init)
			*is_error = true;
	if (type == e_init && next_type == e_subshell_start)
		*is_error = true;
}

int	validate_operator_tokens(\
t_token_elem *now_token, t_token_elem *next_token, bool is_head)
{
	t_token_type	type;
	t_token_type	next_type;
	bool			is_syntax_err;

	if (!now_token)
		return (FAILURE);
	type = now_token->type;
	next_type = e_null;
	if (next_token)
		next_type = next_token->type;
	is_syntax_err = false;
	validate_operator_list_token(type, next_type, &is_syntax_err, is_head);
	validate_pipeline_commands_token(type, next_type, &is_syntax_err, is_head);
	validate_subshell_token(type, next_type, &is_syntax_err, is_head);
	if (!is_syntax_err)
		return (SUCCESS);
	if (!next_token)
		ft_dprintf(STDERR_FILENO, \
		"minishell: syntax error near unexpected token `newline'\n");
	else
		ft_dprintf(STDERR_FILENO, \
		"minishell: syntax error near unexpected token `%s'\n", \
		next_token->word);
	return (FAILURE);
}
