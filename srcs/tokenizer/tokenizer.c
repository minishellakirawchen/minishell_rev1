/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:28 by takira            #+#    #+#             */
/*   Updated: 2023/01/19 15:51:33 by takira           ###   ########.fr       */
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



// add tokentype
// disconnect to control opes ["hoge"]=[|] -> ["hoge"],[|]
int	set_elem_type_if_word(t_list **tokenlist_head)
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

void	set_elem_type_if_operator(t_token_elem **now_token)
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
//	printf("token:%s ->", (*now_token)->word);
	while (operators[idx])
	{
		if (is_same_str(operators[idx], (*now_token)->word))
		{
			(*now_token)->type = idx;
			printf("token:%s, type:%zu\n", operators[idx], idx);
			return ;
		}
		idx++;
	}
}


// OK echo hello; < infile
// OK echo hello; > outfile
// validate relationship now and next

// [|,||,&&] + [<,<<,>,>>,(,init,]
// TODO: bash-3.2$ <>out
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
//	printf("word:%s\n", now_token->word);
	if (is_tokentype_semicolon(type))
		if (is_head || is_tokentype_pipe_or_and(next_type) || next_type == e_subshell_end)
			is_err = true;
	if (!is_err && is_tokentype_pipe_or_and(type))
		if (is_head || is_tokentype_semicolon(next_type) ||
				is_tokentype_pipe_or_and(next_type) || next_type == e_subshell_end || next_type == e_nothing)
			is_err = true;
	if (!is_err && type == e_subshell_start)
		if (is_tokentype_semicolon(next_type) ||
				is_tokentype_pipe_or_and(next_type) || next_type == e_subshell_end || next_type == e_nothing)
			is_err = true;
	if (!is_err && type == e_subshell_end)
		if (is_head || is_tokentype_subshell(next_type) || next_type == e_init)
			is_err = true;
	if (!is_err && is_tokentype_redirection(type))
		if (is_tokentype_semicolon(next_type) ||
			is_tokentype_pipe_or_and(next_type) ||
				is_tokentype_redirection(next_type) || is_tokentype_subshell(next_type) || next_type == e_nothing)
			is_err = true;
	if (!is_err && type == e_init)
		if (next_type == e_subshell_start)
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

// syntax error
// <> filename
int	arrange_and_validate_token_list(t_list *tokenlist_head)
{
	t_list			*node;
	t_token_elem	*token;
	t_token_elem	*next_token;
	bool			is_head;

	if (!tokenlist_head)
		return (FAILURE);

	node = tokenlist_head;
	// validate control sign
	while (node)
	{
		token = node->content;
		if (validate_operator_sign(token) == FAILURE)
			return (FAILURE);
		set_elem_type_if_operator(&token);
		if (validate_quote(token) == FAILURE)
			return (FAILURE);
		node = node->next;
	}

	// validate syntax
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

		if (validate_syntax(token, next_token, is_head) == FAILURE)
			return (FAILURE);

		node = node->next;
		if (is_head)
			is_head = false;
	}

	set_elem_type_if_word(&tokenlist_head);

	// arrange

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
