/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print_wildcard_validlist.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 20:35:59 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 20:41:02 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print_wildcard_valid_list(int *list, size_t len)
{
	size_t	idx;

	idx = 0;
	printf("(");
	while (idx < len)
	{
		printf("%d", list[idx]);
		idx++;
		if (idx < len)
			printf(",");
	}
	printf(")\n");
}

void	print_wildcard_valid_list(t_token_elem *token)
{
	const size_t	len = ft_strlen_ns(token->word);
	size_t			idx;

	idx = 0;
	if (!token->wildcard_valid_flag)
		return ;
	ft_dprintf(STDERR_FILENO, "(");
	while (token->wildcard_valid_flag && idx < len)
	{
		ft_dprintf(STDERR_FILENO, "%d", token->wildcard_valid_flag[idx]);
		idx++;
		if (idx < len)
			ft_dprintf(STDERR_FILENO, ",");
	}
	ft_dprintf(STDERR_FILENO, ")");
}

static void	print_content(t_list_bdi *node, t_token_elem *token)
{
	const char		*type[] = {";", "|", "||", "&&", "(", ")", \
	"<", ">", ">>", "<<", "f", "e", "w", "i", NULL};

	ft_dprintf(STDERR_FILENO, "[%s]", token->word);
	ft_dprintf(STDERR_FILENO, "%s", type[token->type]);
	if (token->quote_chr == CHR_DOUBLE_QUOTE)
		ft_dprintf(STDERR_FILENO, "\"");
	if (token->quote_chr == CHR_SINGLE_QUOTE)
		ft_dprintf(STDERR_FILENO, "\'");
	print_wildcard_valid_list(token);
	ft_dprintf(STDERR_FILENO, "%d", token->subshell_depth);
	if (token->is_connect_to_next && node->next)
		ft_dprintf(STDERR_FILENO, "=");
	if (!token->is_connect_to_next && node->next)
		ft_dprintf(STDERR_FILENO, ",");
}

void	debug_print_tokens(t_list_bdi *head, char *str)
{
	t_list_bdi		*node;
	t_token_elem	*token;

	if (str)
		ft_dprintf(STDERR_FILENO, "#%-15s:", str);
	node = head;
	while (node)
	{
		token = node->content;
		print_content(node, token);
		node = node->next;
	}
	ft_dprintf(STDERR_FILENO, "\n");
}
