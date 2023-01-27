/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 17:07:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/23 21:58:34 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	validate_operator_sign(t_token_elem *now_token)
{
	// same as e_token_type idx より良い方法はないものか...
	const char		*operators[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", NULL};
	size_t		idx;

	if (!now_token)
		return (FAILURE);
	if (now_token->is_quoted)
		return (SUCCESS);
	if (!is_str1chrs_in_str2(STR_OPERATOR, now_token->word))
		return (SUCCESS);
	idx = 0;
	while (operators[idx])
	{
		if (is_same_str(operators[idx], now_token->word))
			return (SUCCESS);
		idx++;
	}
	ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `%s'\n", now_token->word);
	return (FAILURE);
}
