/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_operator_list_helper.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 08:43:07 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 08:43:19 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	is_pipeline_token(t_token_elem *token_elem, ssize_t	subshell_depth)
{
	if (!token_elem)
		return (false);
	if (!(is_tokentype_operator(token_elem->type) \
	&& token_elem->subshell_depth == subshell_depth))
		return (true);
	return (false);
}

void	delete_operator_token(t_list_bdi **operator_token)
{
	if (!operator_token || !*operator_token)
		return ;
	ft_lstdelone_bdi(operator_token, free_token_elem);
	*operator_token = NULL;
}

