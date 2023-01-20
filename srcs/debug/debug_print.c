/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:21:33 by takira            #+#    #+#             */
/*   Updated: 2023/01/20 10:33:29 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void debug_print_2d_arr(char **arr, char *str)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	if (str)
		ft_dprintf(STDERR_FILENO, "#%-15s:", str);
	ft_dprintf(STDERR_FILENO, "{");
	while (arr[i])
	{
		ft_dprintf(STDERR_FILENO, "\"%s\"", arr[i++]);
		if (arr[i])
			ft_dprintf(STDERR_FILENO, ", ");
	}
	ft_dprintf(STDERR_FILENO, "}\n");
}

void	debug_print_token_word(t_list *head, char *str)
{
	t_list			*node;
	t_token_elem	*token;
	const char	*type[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", "file", "eof", "word", "init", NULL};

	if (str)
		ft_dprintf(STDERR_FILENO, "#%-15s:", str);
	node = head;
	while (node)
	{
		token = node->content;

		ft_dprintf(STDERR_FILENO, "[%s]", token->word);
		ft_dprintf(STDERR_FILENO, "%s", type[token->type]);
		if (is_tokentype_subshell(token->type))
			ft_dprintf(STDERR_FILENO, "%2d", token->parenthesis_no);
		ft_dprintf(STDERR_FILENO, "%c", token->is_quoted ? token->word[0] : '\0');

		if (token->is_connect_to_next_word && node->next)
			ft_dprintf(STDERR_FILENO, "=");
		if (!token->is_connect_to_next_word && node->next)
			ft_dprintf(STDERR_FILENO, ",");
		node= node->next;
	}
	ft_dprintf(STDERR_FILENO, "\n");
}
