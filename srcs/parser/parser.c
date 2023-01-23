/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:48 by takira            #+#    #+#             */
/*   Updated: 2023/01/23 18:17:12 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "paeser.h"

//             [root]
//               ┃
//             [&&]
// ┏━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┓
//[a]                         [sub-shell]
//                  ┏━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┓
//                [&&]                           [&&]
//       ┏━━━━━━━━━━┻━━━━━━━━━━━┓             ┏━━━┻━━━━┓
//  [sub-shell]            [sub-shell]       [g]       null
//  ┏━━━━┻━━━━┓           ┏━━━━━┻━━━━━━┓
//[b]        null       [||]           null
//                   ┏━━━┻━━━━┓
//                  [c]      [|]
//                        ┏━━━┻━━━━┓
//                       [d]   [sub-shell]
//                             ┏━━━┻━━━━┓
//                            [f]       [f]


size_t	count_subshell_in_height(t_list *list_node, int search_height)
{
	size_t			cnt;
	t_token_elem	*token;

	if (!list_node)
		return (0);
	cnt = 0;
	while (list_node)
	{
		token = list_node->content;
		if (token->type == e_subshell_start && token->depth == search_height)
			cnt++;
		list_node = list_node->next;
	}
	return (cnt);
}

t_node_kind set_node_kind(t_token_type token_type)
{
	if (is_tokentype_semicolon(token_type) || is_tokentype_pipe_or_and(token_type))
		return (e_operator);
	if (is_tokentype_subshell(token_type))
		return (e_subshell);
	return (e_pipeline);
}

char	*get_node_kind(t_node_kind kind)
{
	if (kind == e_operator)
		return ("operator");
	if (kind == e_subshell)
		return ("subshell");
	return ("pipeline");
}

t_exec_list *create_execlist_node(t_node_kind kind, t_list *token_head, t_exec_list **prev, t_exec_list **next)
{
	t_exec_list	*new_node;
	t_exec_list	*set_prev;
	t_exec_list	*set_next;

	errno = 0;
	new_node = (t_exec_list *)malloc(sizeof(t_exec_list));
	if (!new_node)
	{
		perror("malloc");
		return (NULL);
	}
	set_prev = NULL;
	set_next = NULL;
	if (prev && *prev)
	{
		set_prev = *prev;
		set_prev->next = new_node;
	}
	if (next && *next)
		set_next = *next;
	new_node->node_kind = kind;
	new_node->token_list_head = token_head;
	new_node->prev = set_prev;
	new_node->next = set_next;
	return (new_node);
}


// echo hello | echo world && cat Makefile | grep a || (cd /bin && ls) > out; cat <infile;
// A && B || C ; D
// [root]
//   |
//  [pipeline]:echo hello | echo world
//  [&&]
//  [pipeline]:echo hello | echo world

// 初めはpipelineを作る
// operatorがきたらpipeline->operator

t_exec_list	*create_operator_list(t_list **tokenlist_head)
{
	t_list			*token_node;
	t_list			*next_node;
	t_list			*popped_node;
	t_exec_list		*operator_node;
	t_exec_list		*exec_head;
	t_exec_list		*pipeline_node;
	t_token_elem	*token_elem;

	if (!tokenlist_head || !*tokenlist_head)
		return (NULL);

	exec_head = create_execlist_node(e_head, NULL, NULL, NULL);
	pipeline_node = create_execlist_node(e_pipeline, NULL, &exec_head, NULL);
	if (!exec_head || !pipeline_node)
		return (NULL); //TODO:all free
	token_node = *tokenlist_head;
	while (token_node)
	{
		token_elem = token_node->content;
		next_node = token_node->next;
		if (is_tokentype_pipeline(token_elem->type) || token_elem->depth > 0)
		{
			popped_node = token_node;
			popped_node->next = NULL;
			ft_lstadd_back(&(pipeline_node->token_list_head), popped_node);
		}
		else
		{
			operator_node = create_execlist_node(e_operator, NULL, &pipeline_node, NULL);
			if (!operator_node)
				return (perror_ret_nullptr("malloc")); // TODO:all free
			operator_node->token_type = token_elem->type;
			pipeline_node = create_execlist_node(e_pipeline, NULL, &operator_node, NULL);
			if (!pipeline_node)
				return (perror_ret_nullptr("malloc")); // TODO:all free
			ft_lstdelone(token_node, free_token_elem);
		}
		token_node = next_node;
	}
	return (exec_head);
}

/* create_command_list_from_pipeline_node(t_exec_list **pipeline_node) */
/*
// command_list->subshell_token_list : token list in subshell w/o ( and ) which same depth
// command_list->pipeline_token_list : token list in until pipe

// exec_list->token_list[i]				: cat Makefile    |  grep a       | (echo hello) > out1 | (pwd && (cd /bin && pwd) || echo hoge) >> out2 < in1
// exec_list->token_list[i]->pipeline	: command_list[0]->command_list[1]->command_list[2]    ->command_list[3]

// command_list[0]->subshell_token_list	: NULL
// command_list[0]->pipeline_token_list	: cat Makefile

// command_list[1]->subshell_token_list	: NULL
// command_list[1]->pipeline_token_list	: grep a

// command_list[2]->subshell_token_list	: echo hello
// command_list[2]->pipeline_token_list	: > out1

// command_list[3]->subshell_token_list	: pwd && (cd /bin && pwd) || echo hoge
// command_list[3]->pipeline_token_list	: >> out2 < in1
*/
int create_command_list_from_pipeline_node(t_exec_list **pipeline_node)
{
	t_list			*pipeline_tokens;
	t_token_elem	*token;
	t_list			*prev;
	t_list			*popped_token;
	t_list			*next;
	ssize_t			subshell_depth;

	t_command_list	*command_list;
	t_list			*new_pipeline;
	//new_pipeline = lstnew(command_list)
	//lstadd_back(&(*pipeline_node)->command_list, new_pipeline)

	if (!pipeline_node || !*pipeline_node || !(*pipeline_node)->token_list_head)
		return (FAILURE);

	command_list = (t_command_list *)malloc(sizeof(t_command_list));
	if (!command_list)
		return (FAILURE);
	command_list->type = e_init_kind;
	command_list->commands = NULL;
	command_list->pipeline_token_list = NULL;
	command_list->subshell_token_list = NULL;

	pipeline_tokens = (*pipeline_node)->token_list_head;
	(*pipeline_node)->pipeline = NULL;//for lstadd_back;

	while (pipeline_tokens)
	{
		token = pipeline_tokens->content;
		if (token->type == e_subshell_start)
		{
			command_list->type = e_subshell;
			// ( )を取り除いてsubshell_token_listに入れる
			// (hoge), (hoge)>out
			subshell_depth = token->depth;
			// 今のdepthよりも小さなdepthの|まで実行
			while (pipeline_tokens && token->depth >= subshell_depth)
			{
				next = pipeline_tokens->next;
				if (is_tokentype_subshell(token->type) && token->depth == subshell_depth)
					ft_lstdelone(pipeline_tokens, free_token_elem); // delete "(", ")"
				else
				{
					popped_token = pipeline_tokens;
					popped_token->next = NULL;
					pipeline_tokens = NULL;
					ft_lstadd_back(&command_list->subshell_token_list, popped_token);
				}
				pipeline_tokens = next;
				if (pipeline_tokens)
					token = pipeline_tokens->content;
			}
		}
		if (token && token->type != e_ope_pipe)
		{
			if (command_list->type == e_init_kind)//not subshell
				command_list->type = e_commands;
			while (pipeline_tokens && token->type != e_ope_pipe)
			{
				next = pipeline_tokens->next;
				// command_list, command_list->pipeline_tolen_list+=pipeline_tokens;
				popped_token = pipeline_tokens;
				popped_token->next = NULL;
				pipeline_tokens = NULL;
				ft_lstadd_back(&command_list->pipeline_token_list, popped_token);
				pipeline_tokens = next;
				if (pipeline_tokens)
					token = pipeline_tokens->content;
			}
		}
		new_pipeline = ft_lstnew(command_list);
		if (!new_pipeline)
			return (FAILURE); //TODO
		ft_lstadd_back(&(*pipeline_node)->pipeline, new_pipeline);
		command_list = (t_command_list *)malloc(sizeof(t_command_list));
		if (!command_list)
			return (FAILURE);
		command_list->type = e_init_kind;
		command_list->commands = NULL;
		command_list->pipeline_token_list = NULL;
		command_list->subshell_token_list = NULL;
		prev = pipeline_tokens;
		if (pipeline_tokens)
			pipeline_tokens = pipeline_tokens->next;
		ft_lstdelone(prev, free_token_elem); //delete pipe node
	}
	(*pipeline_node)->token_list_head = NULL;
	return (SUCCESS);
}

int	create_command_list(t_exec_list **exec_list_head)
{
	t_exec_list	*node;

	if (!exec_list_head || !*exec_list_head)
		return (FAILURE);
	node = *exec_list_head;
	while (node)
	{
		// operatorなら飛ばす
		// そうでなければlist=create_command_list(node->tokenlist)でpipeをnextとする線形リストを作成
		// node->command_list_head = command_list;
		// node->redirect_list_head = redirect_list;
		// TODO: subshellの扱いは？
		// echo hello > out (subshell) これは不可？
		// (subshell) > out
		// | (subshell) | これは問題なさそう
		// (hoge; huga && piyo)
		// subshellはforkするからpipeと同じ扱いとする
		// (内)は整形せず、subshellフラグを立てて、parsing->execへ渡す
		if (node->node_kind == e_pipeline)
			if (create_command_list_from_pipeline_node(&node) == FAILURE)
				return (FAILURE); //TODO:free?
		node = node->next;
	}
	return (SUCCESS);
}

int	parsing_token_list(t_info *info)
{
	t_exec_list	*exec_list_head;

	if (!info || !info->tokenlist_head)
		return (FAILURE);

	// operator list
	exec_list_head= create_operator_list(&info->tokenlist_head);
	if (!exec_list_head)
	{
		ft_dprintf(STDERR_FILENO, "fail to create_operator_list\n");
		return (FAILURE);
	}
	// print
//	debug_print_exec_list(exec_list_head, "operator_list");


	if (create_command_list(&exec_list_head) == FAILURE)
	{
		ft_dprintf(STDERR_FILENO, "fail to create_command_list\n");
		return (FAILURE);
	}

	debug_print_exec_list(exec_list_head, "command_list");

	return (SUCCESS);
}
































