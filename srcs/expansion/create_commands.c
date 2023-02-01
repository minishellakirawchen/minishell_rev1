/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:22:41 by takira            #+#    #+#             */
/*   Updated: 2023/02/01 20:48:04 by takira           ###   ########.fr       */
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
		if(!token_elem->is_connect_to_next_word)
			idx++;
		ft_lstdelone_bdi(&popped_list, free_token_elem);
	}
	return (commands);
}

int	quote_removal_or_re_tokenize(t_list_bdi **expanded_token_list, t_list_bdi *popped_node)
{
	t_token_elem	*splitted_token;
	t_list_bdi		*space_splitted_list;
	t_token_elem	*token_elem;

	if (!expanded_token_list || !popped_node)
		return (FAILURE);
	token_elem = popped_node->content;
	/* if is_quoted, quote removal */
	if (token_elem->is_quoted)
	{
		if (remove_quotes(&token_elem->word) == FAILURE)
			return (FAILURE);
		ft_lstadd_back_bdi(expanded_token_list, popped_node);
	}
	else
	{
		/* is_connect_to_next */
		/* re tokenize by `space` */
		space_splitted_list = get_delim_splitted_tokenlist(token_elem->word, STR_SPACE, STR_QUOTE);
		if (!space_splitted_list)
			return (FAILURE); //TODO:free
		splitted_token = space_splitted_list->content;
		splitted_token->is_connect_to_next_word = token_elem->is_connect_to_next_word;

		ft_lstadd_back_bdi(expanded_token_list, space_splitted_list);
		ft_lstdelone_bdi(&popped_node, free_token_elem);
	}
	return (SUCCESS);
}

//TODO
/* wildcard is not expand if export assign */
/* export hoge=* */
//             ^
int create_expanded_token_list(t_list_bdi **expanded_token_list, t_list_bdi **src_tokens, t_info *info)
{
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;
//	bool			is_command_export;

	if (!expanded_token_list || !src_tokens || !info)
		return (FAILURE);
	popped_node = ft_lstpop_bdi(src_tokens);
	token_elem = popped_node->content;
	/* if is_expandable, expand vare */
	if (is_expandable_var_in_str(token_elem->word, token_elem->quote_chr))
	{
		token_elem->word = get_expanded_str(token_elem->word, info);
		if (!token_elem->word)
			return (FAILURE);
	}
	/* expand wildcard as space separated string */
	if (is_expandable_wildcard_in_str(token_elem->word, token_elem->is_quoted))
	{
		token_elem->word = get_expand_wildcard(token_elem->word);
		if (!token_elem->word)
			return (FAILURE);
	}
	if (quote_removal_or_re_tokenize(expanded_token_list, popped_node) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	expand_var_in_cmd_and_create_cmds_from_tokens(t_command_info **cmd_list, t_info *info)
{
	t_list_bdi		*expanded_token_list;
	char			**commands;

	if (!cmd_list || !*cmd_list || !info)
		return (FAILURE);
//	debug_print_tokens((*cmd_list)->pipeline_token_list, "before expanded token");

	/* expand -> quote removal -> space split -> add expanded_token_list; */
	expanded_token_list = NULL;
	while ((*cmd_list)->pipeline_token_list)
		if (create_expanded_token_list(&expanded_token_list, &(*cmd_list)->pipeline_token_list, info) == FAILURE)
			return (FAILURE);//TODO;free
	commands = create_commands_from_token_list(&expanded_token_list);
	(*cmd_list)->commands = commands;

	return (SUCCESS);
}
