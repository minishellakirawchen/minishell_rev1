/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrange_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 17:07:57 by takira            #+#    #+#             */
/*   Updated: 2023/01/19 21:35:54 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static void	delete_empty_elem(t_list **tokenlist_head);
static int	valid_and_set_control_operator(t_list **tokenlist_head);
static int	validate_syntax(t_list *tokenlist_head);
static int	validate_subshell_parenthesis_pairs(t_list *tokenlist_head);
static void	set_elem_type_if_operator(t_list **tokenlist_head);
static int	set_elem_type_if_word(t_list **tokenlist_head);

// syntax error
// <> filename
int	arrange_and_validate_token_list(t_list **tokenlist_head)
{
	if (!tokenlist_head || !*tokenlist_head)
		return (FAILURE);
	if (valid_and_set_control_operator(tokenlist_head) == FAILURE)
		return (FAILURE);
	set_elem_type_if_operator(tokenlist_head);
	if (validate_quote(*tokenlist_head) == FAILURE)
		return (FAILURE);
	if (validate_subshell_parenthesis_pairs(*tokenlist_head) == FAILURE)
		return (FAILURE);
	debug_print_token_word(*tokenlist_head, "set opes");
	delete_empty_elem(tokenlist_head);
	if (validate_syntax(*tokenlist_head) == FAILURE)
		return (FAILURE);
	set_elem_type_if_word(tokenlist_head);

	if (ft_lstsize(*tokenlist_head) == 0)
		return (FAILURE);
	return (SUCCESS);
}

static int validate_subshell_parenthesis_pairs(t_list *tokenlist_head)
{
	t_list			*node;
	t_token_elem	*token;
	int				cnt;

	if (!tokenlist_head)
		return (FAILURE);
	node = tokenlist_head;
	cnt = 0;
	while (node)
	{
		token = node->content;
		if (token->type == e_subshell_start)
			cnt++;
		if (token->type == e_subshell_end)
			cnt--;
		node = node->next;
	}
	if (cnt == 0)
		return (SUCCESS);
	if (cnt > 0)
		ft_dprintf(STDERR_FILENO, "minishell: unclosed parenthesis `('\n");
	else
		ft_dprintf(STDERR_FILENO, "minishell: unclosed parenthesis `)'\n");
	return (FAILURE);
}

static int validate_syntax(t_list *tokenlist_head)
{
	t_list			*node;
	t_token_elem	*token;
	t_token_elem	*next_token;
	bool			is_head;

	node = tokenlist_head;
	next_token = NULL;
	is_head = true;
	while (node)
	{
		token = node->content;
		if (node->next)
			next_token = node->next->content;
		else
			next_token = NULL;
		if (validate_context(token, next_token, is_head) == FAILURE)
			return (FAILURE);
		node = node->next;
		if (is_head)
			is_head = false;
	}
	return (SUCCESS);
}

// TODO: separate set_elem_type_if_operator
static int	valid_and_set_control_operator(t_list **tokenlist_head)
{
	t_list			*node;
	t_token_elem	*token;

	node = *tokenlist_head;
	// validate control sign
	// error: <<<, ;;, |||, &&&, etc.
	while (node)
	{
		token = node->content;
		if (validate_operator_sign(token) == FAILURE)
			return (FAILURE);
		node = node->next;
	}
	return (SUCCESS);
}

static void	set_elem_type_if_operator(t_list **tokenlist_head)
{
	const char		*operators[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", NULL};
	size_t			idx;
	t_list			*node;
	t_token_elem	*now_token;

	if (!tokenlist_head || !*tokenlist_head)
		return ;
	node = *tokenlist_head;
	while (node)
	{
		now_token = node->content;
		if (!now_token->is_quoted && is_str1chrs_in_str2(STR_OPERATOR, now_token->word))
		{
			idx = 0;
			while (operators[idx])
			{
				if (is_same_str(operators[idx], now_token->word))
					now_token->type = idx;
				idx++;
			}
		}
		node = node->next;
	}
}

static int	set_elem_type_if_word(t_list **tokenlist_head)
{
	t_list			*node;
	t_token_elem	*token;
	t_token_elem	*next_token;

	if (!tokenlist_head || !*tokenlist_head)
		return (FAILURE);

	node = *tokenlist_head;
	while (node)
	{
		token = node->content;
		if (node->next)
			next_token = node->next->content;

		// disconnect to control operator
		if (token->is_connect_to_next_word && next_token && next_token->type != e_init)
			token->is_connect_to_next_word = false;
		if (next_token && is_tokentype_redirection(token->type))
		{
			if (next_token->type != e_init)
				return (FAILURE);
			if (token->type == e_redirect_in || token->type == e_redirect_out || token->type == e_redirect_append)
				next_token->type = e_file;
			if (token->type == e_heredoc)
				next_token->type = e_heredoc_eof;
		}
		if (token->type == e_init)
			token->type = e_word;
		node = node->next;
	}
	return (SUCCESS);
}

static void	delete_empty_elem(t_list **tokenlist_head)
{
	t_list			*node;
	t_list			*prev;
	t_token_elem	*token;
	bool			del_this_node;

	if (!tokenlist_head || !*tokenlist_head)
		return ;
	node = *tokenlist_head;
	prev = NULL;
	while (node)
	{
		del_this_node = false;
		token = node->content;
		if (token->is_quoted && ft_strlen_ns(token->word) == 2)
			del_this_node = true;
		if (ft_strlen_ns(token->word) == 0)
			del_this_node = true;
		if (!del_this_node)
		{
			prev = node;
			node = node->next;
			continue ;
		}
		if (prev)
			prev->next = node->next;
		else
			*tokenlist_head = node->next;
		ft_lstdelone(node, free_token_elem);
		if (prev)
			node = prev->next;
		else
			node = *tokenlist_head;

	}
}
