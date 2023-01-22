/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_token_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 17:05:45 by takira            #+#    #+#             */
/*   Updated: 2023/01/22 14:18:29 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int is_tokentype_semicolon(t_token_type type)
{
	return (type == e_ope_semicolon);
}

int is_tokentype_pipe_or_and(t_token_type type)
{
	return (type == e_ope_pipe || type == e_ope_or || type == e_ope_and);
}

int is_tokentype_redirection(t_token_type type)
{
	return (type == e_redirect_in || type == e_redirect_out || type == e_redirect_append || type == e_heredoc);
}

int is_tokentype_subshell(t_token_type type)
{
	return (type == e_subshell_start || type == e_subshell_end);
}

// BNF
int is_tokentype_operator(t_token_type type)
{
	return (is_tokentype_semicolon(type) || type == e_ope_or || type == e_ope_and);
}

int is_tokentype_pipeline(t_token_type type)
{
	return (!is_tokentype_operator(type));
}
