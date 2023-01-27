/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:13 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:35:30 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

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
//    t_command_info command_list
//      t_list pipeline_token_list  : token list, echo(word)->hello(word)->world(word)->NULL
//      char **commands             : expanded commands {"echo" "hello", "world", NULL} <-create this by token_list, and clear tokens

// input pipeline is type=pipeline
// type=subshell is expanded in execution->parsing process

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

int	expand_variable(t_exec_list **pipeline, t_info *info)
{
	t_list_bdi		*command_list_node;
	t_command_info	*command_list;

	if (!pipeline || !info)
		return (FAILURE);
	command_list_node = (*pipeline)->pipeline_commands;
	while (command_list_node)
	{
		command_list = command_list_node->content;
		if (create_redirect_list_from_pipeline_tokens(&command_list, info) == FAILURE)
			return (FAILURE);
		if (create_commands_from_pipeline_tokens(&command_list, info) == FAILURE)
			return (FAILURE);
		command_list_node = command_list_node->next;
	}
	return (SUCCESS);
}




