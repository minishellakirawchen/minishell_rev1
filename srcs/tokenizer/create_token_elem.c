/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 22:58:23 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:28:53 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

t_token_elem	*create_token_elem(\
char *word, bool is_connected, bool is_quoted, char quote_chr)
{
	t_token_elem	*new_token;

	new_token = (t_token_elem *)malloc(sizeof(t_token_elem));
	if (!new_token)
	{
		free_token_elem(new_token);
		return (perror_ret_nullptr("malloc"));
	}
	new_token->word = word;
	new_token->type = e_init;
	new_token->is_connect_to_next = is_connected;
	new_token->is_quoted = is_quoted;
	new_token->quote_chr = quote_chr;
	new_token->subshell_depth = -1;
	new_token->wildcard_valid_flag = NULL;
	return (new_token);
}
