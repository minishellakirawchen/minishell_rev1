/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:22:41 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 17:52:14 by takira           ###   ########.fr       */
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

//$key  :not split
//"$key":split

int	create_commands_from_pipeline_tokens(t_command_info **cmd_list, t_info *info)
{
	t_list_bdi	*expanded_token_list;
	t_list_bdi	*popped_node;
	char		*commands;

	if (!cmd_list || !*cmd_list || !info)
		return (FAILURE);
	commands = NULL;

	// expand -> quote removal -> space split -> add expanded_token_list;
	expanded_token_list = NULL;
	while ((*cmd_list)->pipeline_token_list)
	{
		popped_node = ft_lstpop_bdi(&(*cmd_list)->pipeline_token_list);

		//if is_expandable
		//	expand var
		//if is_quoted
		//  quote removal
		//if !is_quoted
		//  re tokenize -> append expanded_token_list

	}
	// expanded_token_list -> char **commands

	(*cmd_list)->commands = (char **)commands;
	return (SUCCESS);
}
