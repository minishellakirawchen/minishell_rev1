/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:13 by takira            #+#    #+#             */
/*   Updated: 2023/01/25 16:09:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
static int	expand_and_create_commands(t_command_list *cmd_list, t_list *envlist);


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
int	expand_variable(t_exec_list **pipeline, t_list *envlist)
{
	t_list			*command_list_node;
	t_command_list	*command_list;

	if (!pipeline || !envlist)
		return (FAILURE);
	command_list_node = (*pipeline)->pipeline_commands;
	while (command_list_node)
	{

		command_list = command_list_node->content;
		if (expand_and_create_commands(command_list, envlist) == FAILURE)
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

int	expand_var_in_tokens(t_list **list_head, t_list *envlist)
{
	t_list	*list_node;

	if (!list_head || !*list_head || !envlist)
		return (SUCCESS);

	list_node = *list_head;
	while (list_node)
	{



		list_node = list_node->next;
	}

	return (SUCCESS);
}

char	*concat_tokens(t_list *list_head)
{
	char	*str;
	if (!list_head)
		return (NULL);

	return (str);
}

/* token node filename or heredoc eof is cleared in function */
char	*get_filename_or_heredoc_eof(t_list **token_get_from, t_list *envlist)
{
	char			*ret_str;
	t_list			*tmp_list;
	t_list			*popped_list;
	t_token_elem	*token_elem;

	if (!token_get_from || !*token_get_from || !envlist)
		return (NULL);
	tmp_list = NULL;
	while (true)
	{
		popped_list = ft_lstpop(token_get_from);
		ft_lstadd_back(&tmp_list, popped_list);
		token_elem = popped_list->content;
		if (!token_elem->is_connect_to_next_word || !*token_get_from)
			break ;
	}
	if (expand_var_in_tokens(&tmp_list, envlist) == FAILURE)
		return (NULL);
	ret_str = concat_tokens(tmp_list);
	if (!ret_str)
		return (NULL);
	ft_lstclear(&tmp_list, free_token_elem);
	return (ret_str);
}

/* redirect token node clear in function */
static int	create_redirect_list_from_pipeline_tokens(t_command_list *cmd_list, t_list *envlist)
{
	t_list			*token_node;
	t_list			*popped_node;
	t_token_elem	*token_elem;
	t_redirect_list	*redirect_list;
	char			*filename_or_heredoc_eof;

	if (!cmd_list || !envlist)
		return (FAILURE);
	token_node = cmd_list->pipeline_token_list;
	while (token_node)
	{
		token_elem = token_node->content;
		if (is_tokentype_redirection(token_elem->type))
		{
			popped_node = ft_lstpop(&token_node);
			ft_lstdelone(popped_node, free_token_elem);
			filename_or_heredoc_eof = get_filename_or_heredoc_eof(&token_node, envlist);
			redirect_list = create_redirect_list(token_elem->type, filename_or_heredoc_eof);
			if (!filename_or_heredoc_eof || !redirect_list)
				return (FAILURE);
			cmd_list->redirect_list = redirect_list;
			return (SUCCESS);
		}
		token_node = token_node->next;
	}
	return (SUCCESS);
}

int	create_commands_from_pipeline_tokens(t_command_list *cmd_list, t_list *envlist)
{
	if (!cmd_list || !envlist)
		return (FAILURE);



	return (SUCCESS);
}


/* pipeline_token_list clear in this func */
static int	expand_and_create_commands(t_command_list *cmd_list, t_list *envlist)
{
	t_list			*token_node;
	t_token_elem	*token_elem;

	if (!cmd_list || !envlist)
		return (FAILURE);
	token_node = cmd_list->pipeline_token_list;
	//count pipeline_token_list
	//expand && split, insert
	//create char **cmds;

	// create redirect_list, expanded
	if (create_redirect_list_from_pipeline_tokens(cmd_list, envlist) == FAILURE)
		return (FAILURE);
	if (create_commands_from_pipeline_tokens(cmd_list, envlist) == FAILURE)
		return (FAILURE);
	cmd_list->pipeline_token_list = NULL;
	return (SUCCESS);
}



































