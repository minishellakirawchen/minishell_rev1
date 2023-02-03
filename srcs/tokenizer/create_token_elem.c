/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 22:58:23 by takira            #+#    #+#             */
/*   Updated: 2023/02/03 22:58:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

t_token_elem	*create_token_elem(char *word)
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
	new_token->is_connect_to_next_word = false;
	new_token->is_quoted = false;
	new_token->quote_chr = '\0';
	new_token->subshell_depth = -1;
	new_token->wildcard_valid_flag = NULL;
	return (new_token);
}
