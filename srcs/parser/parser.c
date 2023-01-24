/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:48 by takira            #+#    #+#             */
/*   Updated: 2023/01/24 14:40:42 by takira           ###   ########.fr       */
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
		return (e_node_operator);
	if (is_tokentype_subshell(token_type))
		return (e_node_subshell);
	return (e_node_pipeline);
}

char	*get_node_kind(t_node_kind kind)
{
	if (kind == e_node_operator)
		return ("operator");
	if (kind == e_node_subshell)
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
		(*prev)->next = new_node;
	}
	if (next && *next)
	{
		set_next = *next;
		(*next)->prev = new_node;
	}
	new_node->node_kind = kind;

	new_node->token_list_head = token_head;
	new_node->pipeline = NULL;

	new_node->token_type = e_init; //tmp

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
	t_list			*popped_node;

	t_token_elem	*token_elem;

	t_exec_list		*exec_head;
	t_exec_list		*pipeline_node;
	t_exec_list		*operator_node;
	ssize_t			subshell_depth;

	if (!tokenlist_head || !*tokenlist_head)
		return (NULL);
	debug_print_token_word(*tokenlist_head, "before create operator list");

	exec_head = create_execlist_node(e_node_head, NULL, NULL, NULL);
	pipeline_node = create_execlist_node(e_node_pipeline, NULL, &exec_head, NULL);

	if (!exec_head || !pipeline_node)
		return (NULL); //TODO:all free
	token_node = *tokenlist_head;

	token_elem = token_node->content;
	subshell_depth = token_elem->depth;
	while (token_node)
	{
		popped_node = ft_lstpop(&token_node);
		token_elem = popped_node->content;
		if (!(is_tokentype_operator(token_elem->type) && token_elem->depth == subshell_depth))
			ft_lstadd_back(&(pipeline_node->token_list_head), popped_node);
		else
		{
			operator_node = create_execlist_node(e_node_operator, NULL, &pipeline_node, NULL);
			if (!operator_node)
				return (perror_ret_nullptr("malloc")); // TODO:all free
			operator_node->token_type = token_elem->type; //for debug print
			ft_lstdelone(popped_node, free_token_elem);
			popped_node = NULL;
			if (token_node)
			{
				pipeline_node = create_execlist_node(e_node_pipeline, NULL, &operator_node, NULL);
				if (!pipeline_node)
					return (perror_ret_nullptr("malloc")); // TODO:all free
			}
		}
	}
	*tokenlist_head = NULL;
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


t_command_list	*create_command_list_node(void)
{
	t_command_list	*command_list;

	errno = 0;
	command_list = (t_command_list *)malloc(sizeof(t_command_list));
	if (!command_list)
		return (perror_ret_nullptr("malloc"));
	command_list->type = e_node_init;
	command_list->commands = NULL;
	command_list->pipeline_token_list = NULL;
	command_list->subshell_token_list = NULL;
	command_list->redirect_list = NULL;
	return (command_list);
}

// TODO:leaks, 見直し
// pipeline_node=exec_list
// node_kind=pipeline(!=operator)
int create_command_list_from_pipeline_node(t_exec_list **pipeline_node)
{
	t_list			*pipeline_tokens;
	t_token_elem	*token_elem;
	t_list			*popped_token;
	ssize_t			subshell_depth;

	t_command_list	*command_list;
	t_list			*new_pipeline;

	if (!pipeline_node || !*pipeline_node || !(*pipeline_node)->token_list_head)
		return (FAILURE);

	command_list = create_command_list_node();
	if (!command_list)
		return (FAILURE);
		
	pipeline_tokens = (*pipeline_node)->token_list_head;
//	(*pipeline_node)->pipeline = NULL;//for lstadd_back;
	
	while (pipeline_tokens)
	{
		popped_token = ft_lstpop(&pipeline_tokens);
		token_elem = popped_token->content;
		/* subshell */
		if (is_tokentype_subshell(token_elem->type))
		{
			command_list->type = e_node_subshell;
//			printf("popped subshell:%s, depth:%zu\n", token_elem->word, subshell_depth);
			subshell_depth = token_elem->depth;
			ft_lstdelone(popped_token, free_token_elem); // delete (
			while (pipeline_tokens)
			{
				popped_token = ft_lstpop(&pipeline_tokens);
				token_elem = popped_token->content;
				if (token_elem->type == e_subshell_end && token_elem->depth == subshell_depth)
				{
					ft_lstdelone(popped_token, free_token_elem); // delete )
					break ;
				}
				ft_lstadd_back(&command_list->subshell_token_list, popped_token);
			}
			continue ;
		}
		/* command */
		if (token_elem->type != e_ope_pipe)
		{
			if (command_list->type == e_node_init)
				command_list->type = e_node_pipeline;
			ft_lstadd_back(&command_list->pipeline_token_list, popped_token);
			continue ;
		}
		/* pipe */
		// command | comamnd <-
		if (token_elem->type == e_ope_pipe)
		{
			new_pipeline = ft_lstnew(command_list);
			if (!new_pipeline)
				return (FAILURE); //TODO
			ft_lstadd_back(&(*pipeline_node)->pipeline, new_pipeline);
			ft_lstdelone(popped_token, free_token_elem); // delete |
			command_list = create_command_list_node();
			if (!command_list)
				return (FAILURE);
		}
	}
	/* last command line */
	new_pipeline = ft_lstnew(command_list);
	if (!new_pipeline)
		return (FAILURE); //TODO
	ft_lstadd_back(&(*pipeline_node)->pipeline, new_pipeline);

//	ft_lstclear(&(*pipeline_node)->token_list_head, free_token_elem);
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
		if (node->node_kind == e_node_pipeline)
			if (create_command_list_from_pipeline_node(&node) == FAILURE)
				return (FAILURE); //TODO:free?
		node = node->next;
	}
	return (SUCCESS);
}

int	parsing_token_list(t_info *info)
{
	if (!info || !info->tokenlist_head)
		return (FAILURE);
	/* operator list */

	info->execlist_head = create_operator_list(&(info->tokenlist_head));
	if (!info->execlist_head)
	{
		ft_dprintf(STDERR_FILENO, "fail to create_operator_list\n");
		return (FAILURE);
	}
	// print
	debug_print_exec_list(info->execlist_head, "operator_list");

	if (create_command_list(&info->execlist_head) == FAILURE)
	{
		ft_dprintf(STDERR_FILENO, "fail to create_command_list\n");
		return (FAILURE);
	}

	debug_print_exec_list(info->execlist_head, "command_list");

	return (SUCCESS);
}
































