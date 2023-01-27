/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:22:41 by takira            #+#    #+#             */
/*   Updated: 2023/01/27 16:10:32 by takira           ###   ########.fr       */
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

int	create_commands_from_pipeline_tokens(t_command_info **cmd_list, t_info *info)
{
	t_list_bdi		*expanded_token_list;
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;
	char			*commands;
	t_list_bdi		*space_splitted_list;

	if (!cmd_list || !*cmd_list || !info)
		return (FAILURE);
	commands = NULL;

	debug_print_token_word((*cmd_list)->pipeline_token_list, "before expanded token");

	// expand -> quote removal -> space split -> add expanded_token_list;
	expanded_token_list = NULL;
	while ((*cmd_list)->pipeline_token_list)
	{
		popped_node = ft_lstpop_bdi(&(*cmd_list)->pipeline_token_list);
		token_elem = popped_node->content;

		//if is_expandable	-> expand vare
		if (is_expandable_var_in_str(token_elem->word, token_elem->quote_chr))
		{
			printf("word:%s, quote_chr:%c, is_quoted:%d\n", token_elem->word, token_elem->quote_chr, token_elem->is_quoted);
			token_elem->word = get_expanded_str(token_elem->word, info);
			if (!token_elem->word)
				return (FAILURE);
		}
		//if is_quoted		-> quote removal
		if (token_elem->is_quoted)
		{
			if (remove_quotes(&token_elem->word) == FAILURE)
				return (FAILURE);
			ft_lstadd_back_bdi(&expanded_token_list, popped_node);
		}
		else
		{
			space_splitted_list = get_delim_splitted_tokenlist(token_elem->word, STR_SPACE, STR_QUOTE);
			if (!space_splitted_list)
				return (FAILURE); //TODO:free
			ft_lstadd_back_bdi(&expanded_token_list, space_splitted_list);
			ft_lstdelone_bdi(&popped_node, free_token_elem);
		}
	}
	debug_print_token_word(expanded_token_list, "expanded token");


	// expanded_token_list -> char **commands

	(*cmd_list)->commands = (char **)commands;
	return (SUCCESS);
}
























