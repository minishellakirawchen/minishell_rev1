/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:22:41 by takira            #+#    #+#             */
/*   Updated: 2023/02/02 15:49:29 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

// pipeline_token_list
// expand & split -> append expanded_tokens

//bash-3.2$ export a2="cho hello        world"
//bash-3.2$ echo $a2
//cho hello world
//bash-3.2$ e$a2
//hello world
//bash-3.2$

// e$key = echo

//bash-3.2$ e"cho" hello	//hello				-> {"echo", "hello"}
//bash-3.2$ e"cho hoge"	//bash: echo hoge: command not found				-> {"echo hoge"}

//bash-3.2$ echo $a1		//hoge huga			-> {"echo", "[hoge huga]"}
//bash-3.2$ e"cho $a1"		//bash: echo hoge    huga : command not found	-> "e[cho $a1]={"echo hoge   huga"}

//bash-3.2$ export a2="cho hello        world"
//bash-3.2$ echo $a2		//cho hello world	-> {"echo", "[cho hello        world]"}

//bash-3.2$ export a3="echo hello world"
//bash-3.2$ $a3				//hello world		-> {"echo", "hello", "world"}

//bash-3.2$ export a3="echo hello world"
//bash-3.2$ echo $a3		//hello world
//bash-3.2$ echo "$a3"		//hello      world

//bash-3.2$ $a3				//bash: hello: command not found
//bash-3.2$ "$a3"			//bash: hello      world: command not found

//bash-3.2$ export a4="cho     hello       world"
//bash-3.2$ e$a4			//hello world
//bash-3.2$ e"$a4"			//bash: echo     hello       world: command not found


//bash-3.2$ echo abc$b1"ABC   DEF"$b2	//abc123ABC   DEFtest test
//
//abc$b1 ABC   DEF $b2

//$key  :not split
//"$key":split

// hoge=huga, piyo <-2
size_t	get_commands_size(t_list_bdi *list)
{
	size_t			size;
	t_token_elem	*token_elem;

	size = 0;
	while (list)
	{
		token_elem = list->content;
		while (list && token_elem->is_connect_to_next_word)
		{
			list = list->next;
			if (list)
				token_elem = list->content;
		}
		size++;
		list = list->next;
	}
	return (size);
}

char	**create_commands_from_token_list(t_list_bdi **token_list)
{
	t_list_bdi		*popped_list;
	t_token_elem	*token_elem;
	char			**commands;
	size_t			size;
	size_t			idx;

	if (!token_list || !*token_list)
		return (NULL);

	// get_size
	size = get_commands_size(*token_list);
	commands = (char **)ft_calloc(sizeof(char *), size + 1);
	if (!commands)
		return (perror_ret_nullptr("malloc"));
	idx = 0;
	while (*token_list)
	{
		popped_list = ft_lstpop_bdi(token_list);
		token_elem = popped_list->content;
		commands[idx] = concat_dst_to_src(&commands[idx], token_elem->word);
		if (!commands[idx])
			return (perror_ret_nullptr("malloc")); // TODO: free
		idx++;
		ft_lstdelone_bdi(&popped_list, free_token_elem);
	}
	return (commands);
}

/* おそらく不要になる */
int	quote_removal_or_re_tokenize(t_list_bdi **expanded_token_list, t_list_bdi *popped_node)
{
	t_token_elem	*splitted_token;
	t_list_bdi		*space_splitted_list;
	t_token_elem	*token_elem;

	if (!expanded_token_list || !popped_node || !popped_node->content)
		return (FAILURE);
	token_elem = popped_node->content;
	/* if is_quoted, quote removal */
//	printf("1　word:%s, quoted:%d\n", token_elem->word, token_elem->is_quoted);

	if (token_elem->is_quoted)
	{

		if (remove_quotes(&token_elem->word) == FAILURE)
			return (FAILURE);
		ft_lstadd_back_bdi(expanded_token_list, popped_node);
	}
	else
	{
		// splitできない場合は何もしない

		if (!is_str1chrs_in_str2(STR_SPACE, token_elem->word))
			ft_lstadd_back_bdi(expanded_token_list, popped_node);
		else
		{
			/* is_connect_to_next */
			/* re tokenize by `space` */
			space_splitted_list = get_delim_splitted_tokenlist(token_elem->word, STR_SPACE, STR_QUOTE);
			if (!space_splitted_list)
			{
				return (FAILURE); //TODO:free
			}

			splitted_token = space_splitted_list->content;
			splitted_token->is_connect_to_next_word = token_elem->is_connect_to_next_word;

			ft_lstadd_back_bdi(expanded_token_list, space_splitted_list);
			ft_lstdelone_bdi(&popped_node, free_token_elem);
		}
	}
	return (SUCCESS);
}

int expand_var_in_token_word(t_list_bdi **src_tokens, t_info *info)
{
	t_token_elem	*token_elem;
	t_list_bdi		*node;

	if (!src_tokens || !info)
		return (FAILURE);

	node = *src_tokens;
	while (node)
	{
		token_elem = node->content;
		if (is_expandable_var_in_str(token_elem->word, token_elem->quote_chr))
			if (expand_var_in_str(&token_elem->word, info) == FAILURE)
				return (FAILURE);
		node = node->next;
	}
	return (SUCCESS);
}

/* re tokenizeでnode数が増える可能性あり。expanded_token_listに一時保管する */
int remove_quote_or_re_tokenize(t_list_bdi **src_tokens)
{
	t_list_bdi		*popped_node;
	t_list_bdi		*expanded_token_list;

	if (!src_tokens)
		return (FAILURE);

	expanded_token_list = NULL;
	while (*src_tokens)
	{
		popped_node = ft_lstpop_bdi(src_tokens);
		if (quote_removal_or_re_tokenize(&expanded_token_list, popped_node) == FAILURE)
			return (FAILURE);
	}
	*src_tokens = expanded_token_list;
	return (SUCCESS);
}

// expand var
//  ↓
// concat ["hello"]=[world] = [helloworld]
//  ↓
// expand wildcard
//  ↓
// create char **commands

char	**get_expanded_commands(t_list_bdi **token_list, t_info *info)
{
	char	**commands;

	if (!token_list || !info)
		return (NULL);
//	debug_print_tokens((*cmd_list)->pipeline_token_list, "before expanded token");

	debug_print_tokens(*token_list, "before expand");
	if (expand_var_in_token_word(&*token_list, info) == FAILURE)
		return (NULL);
	debug_print_tokens(*token_list, "after expand");
	if (remove_quote_or_re_tokenize(&*token_list) == FAILURE)
		return (NULL);
	debug_print_tokens(*token_list, "after quote removal/re tokenize");
	if (concat_connected_tokens(&*token_list) == FAILURE)
		return (NULL);
	debug_print_tokens(*token_list, "after connect");
	if (expanded_wildcard_to_token_list(&*token_list) == FAILURE)
		return (NULL);
	debug_print_tokens(*token_list, "after wildcard expand");
	commands = create_commands_from_token_list(&*token_list);
	if (!commands)
		return (NULL);
	return (commands);
}


//int	get_expanded_commands(t_command_info **cmd_list, t_info *info)
//{
//	char			**commands;
//
//	if (!cmd_list || !*cmd_list || !info)
//		return (FAILURE);
////	debug_print_tokens((*cmd_list)->pipeline_token_list, "before expanded token");
//
//	debug_print_tokens((*cmd_list)->pipeline_token_list, "before expand");
//	if (expand_var_in_token_word(&(*cmd_list)->pipeline_token_list, info) == FAILURE)
//		return (FAILURE);
//	debug_print_tokens((*cmd_list)->pipeline_token_list, "after expand");
//	if (remove_quote_or_re_tokenize(&(*cmd_list)->pipeline_token_list) == FAILURE)
//		return (FAILURE);
//	debug_print_tokens((*cmd_list)->pipeline_token_list, "after quote removal/re tokenize");
//	if (concat_connected_tokens(&(*cmd_list)->pipeline_token_list) == FAILURE)
//		return (FAILURE);
//	debug_print_tokens((*cmd_list)->pipeline_token_list, "after connect");
//	if (expanded_wildcard_to_token_list(&(*cmd_list)->pipeline_token_list) == FAILURE)
//		return (FAILURE);
//	debug_print_tokens((*cmd_list)->pipeline_token_list, "after wildcard expand");
//	commands = create_commands_from_token_list(&(*cmd_list)->pipeline_token_list);
//	if (!commands)
//		return (FAILURE);
//	(*cmd_list)->commands = commands;
//
//	return (SUCCESS);
//}
