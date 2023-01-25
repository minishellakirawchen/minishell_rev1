/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:13 by takira            #+#    #+#             */
/*   Updated: 2023/01/25 22:51:48 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static int	expand_and_create_commands(t_command_list *cmd_list, t_info *info);


// operator && || ;のように、区切りまでのexpansionにしなければならない
// && || ;がexpandにより生成されることはあるか？
// なければ execution内でexpansion->command_executionする ...(1)
// vvv
//bash-3.2$ export test="echo hello && echo world"
//bash-3.2$ $test           //hello && echo world
// 文字列として展開されるだけ(1)で良さそう

/* expand variable. working example following:
 * ex) [$key]        ->[value]
 *     ["hello"world]->[helloworld]
 *     [good'   bye']->[good   bye]
 *     ["$key$key"]  ->[valuevalue]
 *     ['$key']      ->['$key']
 *     ["'$key'"]    ->['value']
 *     ['"$key"']    ->["$key"]
 * where key=value in environment parameter.
 * */
/* call this function in execution part before command_execute */

// input for expand_variable is "pipeline"
//  t_exec_list pipeline, node_lind=pipeline
//    pipeline1->pipeline2->pipeline3->... ($> pipeline1 &&/||/; pipeline2 &&/||/; pipeline3 ..)
//  t_list pipeline_commands = command_list1->command_list2->.. (command_list1 | command_list2 | ....)

//  commant_list assign content of t_list
//    t_command_list command_list
//      t_list pipeline_token_list  : token list, echo(word)->hello(word)->world(word)->NULL
//      char **commands             : expanded commands {"echo" "hello", "world", NULL} <-create this by token_list, and clear tokens

// input pipeline is type=pipeline
// type=subshell is expanded in execution->parsing process

int	expand_variable(t_exec_list **pipeline, t_info *info)
{
	t_list			*command_list_node;
	t_command_list	*command_list;

	if (!pipeline || !info)
		return (FAILURE);
	command_list_node = (*pipeline)->pipeline_commands;
	while (command_list_node)
	{
		command_list = command_list_node->content;
		if (expand_and_create_commands(command_list, info) == FAILURE)
			return (FAILURE);
		command_list_node = command_list_node->next;
	}
	return (SUCCESS);
}


// pipeline_token_list		->	expand				->	split					->	char **commands
// ------------------------------------------------------------------------------------------------------------------------
// [echo]->[hello]->[world]															{"echo", "hello", "world", NULL}
// [$key]					->	[echo hello world]	->	[echo]->[hello]->[world]	->	{"echo", "hello", "world", NULL}
// ["hello"]=[world]
//
//
//
//bash-3.2$ export e1="echo hello"world'huga'
//bash-3.2$ echo $e1  # echo helloworldhuga
//;
//bash-3.2$ export e2="test   a  $e1'hogehoge'"
//bash-3.2$ echo $e2  # test a echo helloworldhuga'hogehoge'
//
//bash-3.2$ export x1=ech"o hello"
//bash-3.2$ $x1       # hello


// redirect_list: do not split

t_redirect_list	*create_redirect_list(t_token_type type, char *str)
{
	t_redirect_list	*redirect_list;

	errno = 0;
	redirect_list = (t_redirect_list *)malloc(sizeof(t_redirect_list));
	if (!redirect_list)
		return (perror_ret_nullptr("malloc"));
	redirect_list->type = type;
	redirect_list->file = NULL;
	redirect_list->heredoc_eof = NULL;
	if (type == e_file)
		redirect_list->file = str;
	else
		redirect_list->heredoc_eof = str;
	return (redirect_list);
}

// name=value
// name=[_a-zA-z][_a-zA-Z0-9]
// str=$hoo$var$hoge
//      ^^^ ここまでを判定して return (true);
bool	is_name(const char *str)
{
	size_t idx;

	if (!str || !str[0])
		return (false);
	if (str[0] != '_' && !ft_isalpha(str[0]))
		return (false);
	idx = 1;
	while (str[idx] && is_chr_in_str(str[idx], STR_EXPAND_EXIT_STATUS))
	{
		if (str[idx] != '_' && !ft_isalnum(str[idx]))
			return (false);
		idx++;
	}
	return (true);
}
// return true if $? or $name in str, do not depend on validate $name
bool is_expandable(const char *str, char quote_chr)
{
	size_t	idx;

	if (!str || quote_chr == CHR_SINGLE_QUOTE)
		return (false);
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == CHR_DOLLAR && str[idx + 1])
		{
			if (str[idx + 1] == CHR_QUESTION)
				return (true);
			if (is_name(&str[idx + 1]))
				return (true);
		}
		idx++;
	}
	return (false);
}

bool	is_str_expandable_name(const char *str)
{
	if (!str)
		return (false);
	if (str[0] == CHR_DOLLAR && !ft_ispunct(str[1]))
		return (true);
	return (false);
}

bool	is_str_expandable_exitstatus(const char *str)
{
	const size_t	len_flag = ft_strlen_ns(STR_EXPAND_EXIT_STATUS);

	if (ft_strncmp_ns(str, STR_EXPAND_EXIT_STATUS, len_flag) == 0)
		return (true);
	return (false);
}

/*
bool	is_expandable(const char *str, char quote_chr)
{
	return (is_str_expandable_exitstatus(str) || is_expandable_name(str, quote_chr));
}
*/

/* free dest */
char	*concat_dst_to_src(char **dst, char **src)
{
	size_t	dstlen;
	size_t	srclen;
	char	*concat_str;

	if (!dst || !src)
		return (NULL);
	dstlen = ft_strlen_ns(*dst);
	srclen = ft_strlen_ns(*src);
	concat_str = (char *)ft_calloc(sizeof(char), dstlen + srclen + 1);
	if (!concat_str)
		return (perror_ret_nullptr("malloc"));
	ft_strlcat(concat_str, *dst, dstlen + 1);
	ft_strlcat(concat_str, *src, dstlen + srclen + 1);
	free(*dst);
	*dst = NULL;
	return (concat_str);
}

int	expand_exit_status(char **expanded_str, int exit_status)
{
	char	*var;

	if (!expanded_str)
		return (FAILURE);
	var = ft_itoa(exit_status);
	if (!var)
		return (perror_ret_int("malloc", FAILURE));
	*expanded_str = concat_dst_to_src(expanded_str, &var);
	if (*expanded_str)
		return (FAILURE);
	return (SUCCESS);
}

char	*get_name_str(const char *str_start_with_dollar)
{
	const char	*src = str_start_with_dollar;
	char		*name_str;
	size_t		idx;

	if (!src || src[0] != CHR_DOLLAR || !src[1])
		return (NULL);
	idx = 1;
	while (src[idx] && src[idx] == '_' && ft_isalnum(src[idx]))
		idx++;
	name_str = ft_substr(src, 1, idx + 1);
	if (!name_str)
		return (perror_ret_nullptr("malloc"));
	return (name_str);
}

char *get_env_value(const char *search_key, t_list *env_list_head)
{
	const size_t	search_key_len = ft_strlen_ns(search_key);
	t_env_elem		*elem;

	if (search_key_len == 0)
		return ("");
	while (env_list_head)
	{
		elem = env_list_head->content;
		if ((ft_strlen_ns(elem->key) == search_key_len) \
		&& (ft_strncmp_ns(elem->key, search_key, search_key_len) == 0))
			return (elem->value);
		env_list_head = env_list_head->next;
	}
	return ("");
}

// expandable_src is string start with $name or $?
// $
char	*get_expanded_str(char *src, t_info *info)
{
	size_t	idx;
	size_t	skip;
	char	*expanded_str;
	char	*key;
	char	*value;
	char	*skip_str;

	if (!src || !info)
		return (FAILURE);
	expanded_str = NULL;
	idx = 0;
	while (src[idx])
	{
		// $? or $nameまでidx++
		skip = 0;
		while (src[idx + skip] && !(src[idx + skip] == CHR_DOLLAR && is_expandable(&src[idx + skip + 1], '\0')))
			skip++;
		if (skip)
		{
			// idx++した分をexpanded_strへ結合
			skip_str = ft_substr(src, idx, skip);
			expanded_str = concat_dst_to_src(&expanded_str, &skip_str);
			if (!skip_str || !expanded_str)
				return (perror_ret_nullptr("malloc"));
			skip_str = free_1d_alloc(skip_str);
			idx += skip;
		}
		if (!src[idx])
			break ;
		// $? or $name のvalueをexpanded_strへ結合子、$? or $name分idx++
		if (is_str_expandable_exitstatus(&src[idx]))
		{
			if (expand_exit_status(&expanded_str, info->exit_status) == FAILURE)
				return (NULL);
			idx += 2; // $?
		}
		else
		{
			key = get_name_str(&src[idx]);
			value = get_env_value(key, info->envlist_head);
			if (!key | !value)
				return (perror_ret_nullptr("malloc"));
			expanded_str = concat_dst_to_src(&expanded_str, &value);
			idx += ft_strlen_ns(key) + 1; // $key
			key = free_1d_alloc(key);
		}
		idx++;
	}
	free(src);
	printf("expanded_str:%s\n", expanded_str);
	return (expanded_str);
}

int remove_quotes(char **token_word)
{
	if (!token_word || !*token_word)
		return (FAILURE);


	return (SUCCESS);
}

// "$hoge $huga"
// remove "'	//"'hoge'"->'hoge'

// $hoge$huga -> helloworld$huga
// ^^^^^         ^^^^^^^^^^
// ^idx                    ^idx
// pop->addback newlist

// mendokusai...
int	expand_var_in_tokens(t_list **list_head, t_info *info)
{
	t_list			*list_node;
	t_token_elem	*token_elem;

	if (!list_head || !*list_head || !info)
		return (SUCCESS);

	list_node = *list_head;
	while (list_node)
	{
		token_elem = list_node->content;
		if (is_expandable(token_elem->word, token_elem->quote_chr))
		{
			token_elem->word = get_expanded_str(token_elem->word, info);
			if (!token_elem->word)
				return (FAILURE);
			if (token_elem->is_quoted)
				if (remove_quotes(&token_elem->word) == FAILURE)
					return (FAILURE);
		}
		list_node = list_node->next;
	}
	return (SUCCESS);
}

char	*concat_tokens(t_list *list_head)
{
	char			*concat_str;
	t_list			*node;
	t_token_elem	*token_elem;
	size_t			size;

	if (!list_head)
		return (NULL);
	size = 0;
	node = list_head;
	while (node)
	{
		token_elem = node->content;
		size += ft_strlen_ns(token_elem->word);
		node = node->next;
	}
	concat_str = (char *)ft_calloc(sizeof(char), size + 1);
	if (!concat_str)
		return (perror_ret_nullptr("malloc"));
	node = list_head;
	while (node)
	{
		token_elem = node->content;
		ft_strlcat(concat_str, token_elem->word, size + 1);
		node = node->next;
	}
	return (concat_str);
}

/* token node filename or heredoc eof is cleared in function */
char	*get_filename_or_heredoc_eof(t_list **token_get_from, t_info *info)
{
	char			*str_concatted_token;
	t_list			*tmp_list;
	t_list			*popped_list;
	t_token_elem	*token_elem;

	if (!token_get_from || !*token_get_from || !info)
		return (NULL);
	tmp_list = NULL;
	while (*token_get_from)
	{
		popped_list = ft_lstpop(token_get_from);
		ft_lstadd_back(&tmp_list, popped_list);
		token_elem = popped_list->content;
		if (!token_elem->is_connect_to_next_word)
			break ;
	}
	if (expand_var_in_tokens(&tmp_list, info) == FAILURE)
		return (NULL);
	str_concatted_token = concat_tokens(tmp_list);
	if (!str_concatted_token)
		return (NULL);
	ft_lstclear(&tmp_list, free_token_elem);
	return (str_concatted_token);
}

/* redirect token node clear in function */
static int	create_redirect_list_from_pipeline_tokens(t_command_list **cmd_list, t_info *info)
{
	t_list			*popped_node;
	t_token_elem	*token_elem;
	t_redirect_list	*redirect_list;
	t_token_type	type;
	char			*filename_or_heredoc_eof;
	t_list			*command_save;

	if (!cmd_list || !*cmd_list || !info)
		return (FAILURE);
	command_save = NULL;
	while ((*cmd_list)->pipeline_token_list)
	{
		popped_node = ft_lstpop(&(*cmd_list)->pipeline_token_list);
		token_elem = popped_node->content;
		if (is_tokentype_redirection(token_elem->type))
		{
			type = token_elem->type;
			ft_lstdelone_null(&popped_node, free_token_elem);
			filename_or_heredoc_eof = get_filename_or_heredoc_eof(&(*cmd_list)->pipeline_token_list, info);
			redirect_list = create_redirect_list(type, filename_or_heredoc_eof);
			if (!filename_or_heredoc_eof || !redirect_list)
				return (FAILURE);
			(*cmd_list)->redirect_list = redirect_list;
			continue ;
		}
		ft_lstadd_back(&command_save, popped_node);
	}
	(*cmd_list)->pipeline_token_list = command_save;
	return (SUCCESS);
}

int	create_commands_from_pipeline_tokens(t_command_list *cmd_list, t_list *envlist)
{
	if (!cmd_list || !envlist)
		return (FAILURE);



	return (SUCCESS);
}


/* pipeline_token_list clear in this func */
static int	expand_and_create_commands(t_command_list *cmd_list, t_info *info)
{
//	t_list			*token_node;
//	t_token_elem	*token_elem;

	if (!cmd_list || !info)
		return (FAILURE);
//	token_node = cmd_list->pipeline_token_list;

	//count pipeline_token_list
	//expand && split, insert
	//create char **cmds;

	// create redirect_list, expanded
	if (create_redirect_list_from_pipeline_tokens(&cmd_list, info) == FAILURE)
		return (FAILURE);

	/*
	if (create_commands_from_pipeline_tokens(cmd_list, envlist) == FAILURE)
		return (FAILURE);
	cmd_list->pipeline_token_list = NULL;
	*/

	return (SUCCESS);
}


int	expansion(t_info *info)
{
	t_exec_list		*exec_node;

	if (!info)
		return (FAILURE);
	exec_node = info->execlist_head;
	while (exec_node)
	{
		if (expand_variable(&exec_node, info) == FAILURE)
			return (FAILURE);
		exec_node = exec_node->next;
	}
	debug_print_exec_list(info->execlist_head, "expansion");
	return (SUCCESS);
}
































