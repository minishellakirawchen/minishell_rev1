/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:02:36 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 22:08:13 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int				create_command_info_from_pipeline_node(t_exec_list **exec_pipeline_node);
static t_command_info	*create_command_info(void);
int						connect_command_info_to_execlist(t_list_bdi **connect_to, t_command_info *command_info);

// operatorなら飛ばす
// そうでなければlist=create_command_list(node->tokenlist)でpipeをnextとする線形リストを作成
// node->command_list_head = command_list;
// node->redirect_list_head = redirect_list;
// echo hello > out (subshell) これは不可？
// (subshell) > out
// | (subshell) | これは問題なさそう
// (hoge; huga && piyo)
// subshellはforkするからpipeと同じ扱いとする
// (内)は整形せず、subshellフラグを立てて、parsing->execへ渡す
int	create_command_list(t_exec_list **exec_list_head)
{
	t_exec_list	*node;

	if (!exec_list_head || !*exec_list_head)
		return (FAILURE);
	node = *exec_list_head;
	while (node)
	{
		if (node->node_kind == e_node_pipeline)
			if (create_command_info_from_pipeline_node(&node) == FAILURE)
				return (FAILURE); //TODO:free?
		node = node->next;
	}
	return (SUCCESS);
}


/* create_command_info_from_pipeline_node(t_exec_list **pipeline_node) */
/*
// command_list->subshell_token_list : token list in subshell w/o ( and ) which same depth
// command_list->pipeline_token_list : token list in until pipe

// exec_list->token_list[i]				: cat Makefile    |  grep a       | (echo hello) > out1 | (pwd && (cd /bin && pwd) || echo hoge) >> out2 < in1
// exec_list->token_list[i]->pipeline_commands	: command_list[0]->command_list[1]->command_list[2]    ->command_list[3]

// command_list[0]->subshell_token_list	: NULL
// command_list[0]->pipeline_token_list	: cat Makefile

// command_list[1]->subshell_token_list	: NULL
// command_list[1]->pipeline_token_list	: grep a

// command_list[2]->subshell_token_list	: echo hello
// command_list[2]->pipeline_token_list	: > out1

// command_list[3]->subshell_token_list	: pwd && (cd /bin && pwd) || echo hoge
// command_list[3]->pipeline_token_list	: >> out2 < in1
*/

// exec_pipeline_node=exec_list
// node_kind=pipeline_commands(!=operator)
int create_command_info_from_pipeline_node(t_exec_list **exec_pipeline_node)
{
	t_token_elem	*token_elem;
	t_list_bdi		*popped_token;
	t_command_info	*command_info;

	if (!exec_pipeline_node || !*exec_pipeline_node || !(*exec_pipeline_node)->token_list_head)
		return (FAILURE);
	command_info = create_command_info();
	if (!command_info)
		return (FAILURE);
	while ((*exec_pipeline_node)->token_list_head)
	{
		popped_token = ft_lstpop_bdi(&(*exec_pipeline_node)->token_list_head);
		token_elem = popped_token->content;
		/* subshell or commands */
		if (token_elem->type != e_ope_pipe)
		{
			move_tokens_to_command_info(&(*exec_pipeline_node)->token_list_head,
										&command_info, popped_token);
			continue ;
		}
		/* pipe */
		if (connect_command_info_to_execlist(
				&(*exec_pipeline_node)->pipeline_commands, command_info) == FAILURE)
			return (FAILURE);
		ft_lstdelone_bdi(&popped_token, free_token_elem); // delete '|'
		command_info = create_command_info();
		if (!command_info)
			return (FAILURE);
	}
	/* last command line */
	if (connect_command_info_to_execlist(
			&(*exec_pipeline_node)->pipeline_commands, command_info) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

static t_command_info	*create_command_info(void)
{
	t_command_info	*command_info;

	errno = 0;
	command_info = (t_command_info *)malloc(sizeof(t_command_info));
	if (!command_info)
		return (perror_ret_nullptr("malloc"));
	command_info->type = e_node_init;
	command_info->commands = NULL;
	command_info->redirect_list = NULL;
	command_info->redirect_fd[FD_INFILE] = -1;
	command_info->redirect_fd[FD_OUTFILE] = -1;
	command_info->redirect_fd[FD_HEREDOC] = -1;
	command_info->pid = -1;
	command_info->pipeline_token_list = NULL;
	command_info->subshell_token_list = NULL;
	return (command_info);
}

int	connect_command_info_to_execlist(t_list_bdi **connect_to, t_command_info *command_info)
{
	t_list_bdi	*new_pipeline;

	new_pipeline = ft_lstnew_bdi(command_info);
	if (!new_pipeline)
		return (FAILURE); //TODO
	ft_lstadd_back_bdi(connect_to, new_pipeline);
	return (SUCCESS);
}
