/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 17:07:00 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:55:16 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	validate_operator_sign(t_token_elem *now_token)
{
	const char	*operators[] = {\
	";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", NULL};
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
	ft_dprintf(STDERR_FILENO, \
	"minishell: syntax error near unexpected token `%s'\n", now_token->word);
	return (FAILURE);
}

/*
 validate control sign
 error: <<<, ;;, |||, &&&, etc.
*/
int	valid_control_operator(t_list_bdi **tokenlist_head)
{
	t_list_bdi		*node;
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
