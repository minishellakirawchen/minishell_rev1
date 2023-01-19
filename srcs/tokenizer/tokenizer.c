/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:28 by takira            #+#    #+#             */
/*   Updated: 2023/01/19 14:05:09 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

// ft_split_set(char **split, char delim, char *quote_chr)
// もしくは, char *input_lineを前からチェックして切り取り文字が出てきたらsubstrして構造体に入れていく？
// 一旦後者で実装してみる

/* if str1[i] in str2, return true */

// [   aaa bbb cccc | echo hello world "good &"morning && test;cat<Makefile>out]
//  ↓
// [aaa],[bbb],[cccc]		type:word
// [|],					 	type:operator
// [echo],[hello],[world]	type:word
// ["good &"morning]		type:word
// [&&]						type:operator
// [test]					type:word
// [;]						type:operator
// [cat]					type:word
// [<]						type:redirection
// [Makefile]				type:word
// [>]						type:redirection
// [out]					type:word

// add tokentype
// disconnect to control opes ["hoge"]=[|] -> ["hoge"],[|]

int	arrange_token_list(t_info *info)
{
//	t_list			*now_node;
//	t_token_elem	*now_token;
//	t_token_elem	*next_token;

	if (!info || !info->tokenlist_head)
		return (FAILURE);

	/*
	now_node = info->tokenlist_head;
	next_token = NULL;
	while (now_node)
	{
		now_token = now_node->content;
		if (now_node->next)
			next_token = now_node->next->content;


		if (is_str1chrs_in_str2(STR_OPERATOR, now_token->word))
		{
			if (is_str1chrs_in_str2(STR_OPERATOR, now_token->word))
			now_token->type = e_control_operator;
		}
		else
		{
			if (now_token->is_connect_to_next_word && next_token
			&& is_str1chrs_in_str2(STR_OPERATOR, next_token->word))
				now_token->is_connect_to_next_word = false;
		}

		now_node = now_node->next;
	}
	 */
	return (SUCCESS);
}

int	validate_operator_sign(t_token_elem *now_token)
{
	// same as e_token_type idx より良い方法はないものか...
	const char	*operators[] = {";", "|", "||", "&&", "(", ")", "<", "<<", ">", ">>", NULL};
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

void	append_operator_type(t_token_elem **now_token)
{
	const char	*operators[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", NULL};
	size_t		idx;

	if (!now_token || !*now_token)
		return ;
	if ((*now_token)->is_quoted)
		return ;
	if (!is_str1chrs_in_str2(STR_OPERATOR, (*now_token)->word))
		return ;
	idx = 0;
	while (operators[idx])
		if (is_same_str(operators[idx++], (*now_token)->word))
		{
			(*now_token)->type = idx;
			return ;
		}
}

int is_semicolon(t_token_type type)
{
	return (type == e_ope_semicolon);
}

int is_pipe_or_and(t_token_type type)
{
	return (type == e_ope_pipe || type == e_ope_or || type == e_ope_and);
}

int is_redirection(t_token_type type)
{
	return (type == e_redirect_in || type == e_redirect_out || type == e_redirect_append || type == e_heredoc);
}

int is_subshell(t_token_type type)
{
	return (type == e_subshell_start || type == e_subshell_end);
}


// OK echo hello; < infile
// OK echo hello; > outfile
// validate relationship now and next

// [|,||,&&] + [<,<<,>,>>,(,init,]
int	validate_syntax(t_token_elem *now_token, t_token_elem *next_token, bool is_head)
{
	t_token_type	type;
	t_token_type	next_type;
	bool			is_err;

	if (!now_token)
		return (FAILURE);
	type = now_token->type;
	next_type = e_nothing;
	if (next_token)
		next_type = next_token->type;
	is_err = false;

	printf("word:%s\n", now_token->word);
	if (is_semicolon(type))
		if (is_head || is_pipe_or_and(next_type) || next_type == e_subshell_end)
			is_err = true;
	if (!is_err && is_pipe_or_and(type))
		if (is_head || is_semicolon(next_type) || is_pipe_or_and(next_type) || next_type == e_subshell_end || next_type == e_nothing)
			is_err = true;
	if (!is_err && type == e_subshell_start)
		if (is_semicolon(next_type) || is_pipe_or_and(next_type) || next_type == e_subshell_end || next_type == e_nothing)
			is_err = true;
	if (!is_err && type == e_subshell_end)
		if (is_head || is_subshell(next_type) || next_type == e_init)
			is_err = true;
	if (!is_err && is_redirection(type))
		if (is_semicolon(next_type) || is_pipe_or_and(next_type) || is_subshell(next_type) || next_type == e_nothing)
			is_err = true;
	if (!is_err && type == e_init)
		if (is_subshell(next_type))
			is_err = true;

	if (!is_err)
		return (SUCCESS);
	if (next_type == e_nothing)
		ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `newline'\n");
	else
		ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `%s'\n", next_token->word);
	return (FAILURE);
}

int	validate_quote(t_token_elem *now_token)
{
	size_t	word_len;
	char 	*word;

	if (!now_token || !now_token->word)
		return (FAILURE);
	if (!now_token->is_quoted)
		return (SUCCESS);
	word = now_token->word;
	word_len = ft_strlen_ns(word);
	if (word_len == 0)
		return (FAILURE);
	if (ft_strlen_ns(word) > 1 && word[0] == word[word_len - 1])
		return (SUCCESS);
	ft_dprintf(STDERR_FILENO, "minishell: unclosed quote `%c'\n", now_token->word[0]);
	return (FAILURE);
}

int	validate_token_list(t_list *tokenlist_head)
{
	t_list			*now_node;
	t_token_elem	*now_token;
	t_token_elem	*next_token;
	bool			is_head;

	if (!tokenlist_head)
		return (FAILURE);

	now_node = tokenlist_head;
	while (now_node)
	{
		now_token = now_node->content;
		if (validate_operator_sign(now_token) == FAILURE)
			return (FAILURE);
		append_operator_type(&now_token);
		if (validate_quote(now_token) == FAILURE)
			return (FAILURE);
		now_node = now_node->next;
	}

	now_node = tokenlist_head;
	next_token = NULL;
	is_head = true;
	while (now_node)
	{
		now_token = now_node->content;
		if (now_node->next)
			next_token = now_node->next->content;

		if (validate_syntax(now_token, next_token, is_head) == FAILURE)
			return (FAILURE);

		now_node = now_node->next;
		if (is_head)
			is_head = false;
	}
	return (SUCCESS);
}

int	tokenize_input_line(t_info *info, const char *readline_input)
{
	if (!info | !readline_input)
		return (FAILURE);
	// split by space
	//   char **input -> list
	info->tokenlist_head = get_delim_splitted_tokenlist(readline_input,
														STR_SPACE, STR_QUOTE);
	if (!info->tokenlist_head)
		return (FAILURE);

	debug_print_token_word(info->tokenlist_head, "split space");

	// split by control operation and redirection
	//   prev->before_list->next  ->  prev->split1->split2...->next
	if (split_by_operators(&info->tokenlist_head) == FAILURE)
		return (FAILURE);

	debug_print_token_word(info->tokenlist_head, "split opes");

	return (SUCCESS);
}
