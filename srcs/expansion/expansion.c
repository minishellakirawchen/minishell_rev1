/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:13 by takira            #+#    #+#             */
/*   Updated: 2023/02/02 11:11:56 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static int	expand_var_in_redirect_filename(t_command_info **cmd_list, t_info *info);

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

// input for expand_var_and_create_commands_from_tokens is "pipeline"
//  t_exec_list pipeline, node_lind=pipeline
//    pipeline1->pipeline2->pipeline3->... ($> pipeline1 &&/||/; pipeline2 &&/||/; pipeline3 ..)
//  t_list pipeline_commands = command_list1->command_list2->.. (command_list1 | command_list2 | ....)

//  commant_list assign content of t_list
//    t_command_info command_list
//      t_list pipeline_token_list  : token list, echo(word)->hello(word)->world(word)->NULL
//      char **commands             : expanded commands {"echo" "hello", "world", NULL} <-create this by token_list, and clear tokens

// input pipeline is type=pipeline
// type=subshell is expanded in execution->parsing process

int	expand_var_and_create_commands_from_tokens(t_exec_list **pipeline, t_info *info)
{
	t_list_bdi		*command_list_node;
	t_command_info	*command_info;

	if (!pipeline || !info)
		return (FAILURE);
	command_list_node = (*pipeline)->pipeline_commands;
	while (command_list_node)
	{
		command_info = command_list_node->content;
		if (expand_var_in_cmd_and_create_cmds_from_tokens(&command_info, info) == FAILURE)
			return (FAILURE);
		if (expand_var_in_redirect_filename(&command_info, info) == FAILURE)
			return (FAILURE);
		command_list_node = command_list_node->next;
	}
	return (SUCCESS);
}


//bash3.2 0 $ echo "$a1"	//hoge    hoge
//bash3.2 0 $ echo $a1		//hoge hoge

//bash3.2 0 $ echo hello>$a1
//bash: $a1: ambiguous redirect

//bash3.2 1 $ echo hello>"$a1"
//bash3.2 0 $ ls
//hoge    hoge

//bash3.2 0 $ echo hello >'$a1'
//bash3.2 0 $ ls
//$a1

// command_list->redirect_list = heredoc->io->io->heredoc->...
// if type=io, expand and create filename from redirect_list->content=redirect_info->token_list


// echo hello>"$a1"	->$a1 = "hoge    huga"
// echo hello>"a1	->$a1 = "hoge", "huga" -> ambiguous error
/*
static int	expand_var_in_redirect_filename(t_command_info **cmd_list, t_info *info)
{
	t_list_bdi		*redirect_list;
	t_redirect_info	*redirect_info;

	if (!cmd_list || !*cmd_list)
		return (FAILURE);
	redirect_list = (*cmd_list)->redirect_list;
	while (redirect_list)
	{
		redirect_info = redirect_list->content;
		redirect_info->is_ambiguous = false;
		if (redirect_info->io_type == e_redirect_in
		|| redirect_info->io_type == e_redirect_out
		|| redirect_info->io_type == e_redirect_append)
		{
			debug_print_tokens(redirect_info->token_list, "brfore expand redirect");
			if (expand_var_in_tokens(&redirect_info->token_list, info) == FAILURE)
				return (FAILURE);
			if (remove_quote_in_tokens(&redirect_info->token_list) == FAILURE)
				return (FAILURE);
			debug_print_tokens(redirect_info->token_list, "after expand redirect");
			redirect_info->filename = concat_tokens(redirect_info->token_list);
			if (!redirect_info->filename)
				return (FAILURE);//TODO;free

//			if (is_expandable_wildcard_in_redirect())
//			{
//				token_elem->word = get_expand_wildcard(token_elem->word);
//				if (!token_elem->word)
//					return (FAILURE);
//			}

		}
		redirect_list = redirect_list->next;
	}
//	debug_print_redirect_info((*cmd_list)->redirect_list, "expand_var_in_redirect");
	return (SUCCESS);
}
*/

size_t	get_2darray_size(char **array)
{
	size_t	idx;

	idx = 0;
	while (array && array[idx])
		idx++;
	return (idx);
}

static int	expand_var_in_redirect_filename(t_command_info **cmd_list, t_info *info)
{
	t_list_bdi		*redirect_list;
	t_redirect_info	*redirect_info;
	t_list_bdi		*expanded_token_list;
	char			**expanded_retokenized_chars;
	char			*concat_token_str;

	if (!cmd_list || !*cmd_list)
		return (FAILURE);
	redirect_list = (*cmd_list)->redirect_list;
	while (redirect_list)
	{
		redirect_info = redirect_list->content;
		redirect_info->is_ambiguous = false;
		if (redirect_info->io_type == e_redirect_in
			|| redirect_info->io_type == e_redirect_out
			|| redirect_info->io_type == e_redirect_append)
		{
			expanded_token_list = NULL;
			concat_token_str = concat_tokens(redirect_info->token_list);
			if (!concat_token_str)
				return (FAILURE);
			while (redirect_info->token_list)
				if (create_expanded_token_list(&expanded_token_list, &redirect_info->token_list, info) == FAILURE)
					return (FAILURE); //TODO;free
			expanded_retokenized_chars = create_commands_from_token_list(&expanded_token_list);
			debug_print_2d_arr(expanded_retokenized_chars, ">> expand redirect >>");
			if (get_2darray_size(expanded_retokenized_chars) != 1)
			{
				redirect_info->is_ambiguous = true;
				redirect_info->filename = concat_token_str;
			}
			else
			{
				redirect_info->filename = ft_strdup(expanded_retokenized_chars[0]);
				if (!redirect_info->filename)
					return (perror_ret_int("malloc", FAILURE));//TODO:free
				free_1d_alloc(concat_token_str);
			}
			free_2d_alloc((void **)expanded_retokenized_chars);

		}
		redirect_list = redirect_list->next;
	}
//	debug_print_redirect_info((*cmd_list)->redirect_list, "expand_var_in_redirect");
	return (SUCCESS);
}
























