/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:02:48 by takira            #+#    #+#             */
/*   Updated: 2023/01/22 22:52:28 by takira           ###   ########.fr       */
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


// echo hello | echo world && cat Makefile | grep a || (cd /bin && ls) > out; cat <infile;
// {echo hello}->{echo world && cat Makefile}->{grep a}
// {(cd /bin && ls)}->{> out}

// pipeline_node = echo hello | echo world
// split by pipe
// 前から見ていき、command_list->pipeline_tokenに仮保存
// expansion後、char **commandsに整形する。redirect_listはその時作成する？
// subshellならばcommand_list->

// command_list1->command_list2->..
//  command_list->content=command_info
//  command_info->
//create_command_list;
// node++
// if type!=pipe
//		if (subshell)
//			while (type!=pipe)
//	 			token_list_head_for_subshell+=token
//  	else
//  		while (type!=pipe)
//				commands_info+=token
//
// if type==pipe
//  pipeline_node->command_list_head+=command_info;
//
// pipeline_node++;
//
//create_redirect_list;
// node++
//
int create_command_list_from_pipeline_node(t_exec_list **pipeline_node)
{
	t_list			*pipeline_token;
	t_token_elem	*token_elem;
	t_list			*prev;
	t_list			*popped_token;
	t_list			*next;
	ssize_t			depth;

	t_command_list	*command_list;
	t_list			*new_pipeline;
	//new_pipeline = lstnew(command_list)
	//lstadd_back(&(*pipeline_node)->command_list, new_pipeline)

	if (!pipeline_node || !*pipeline_node || (*pipeline_node)->token_list_head)
		return (FAILURE);

	command_list = (t_command_list *)malloc(sizeof(t_command_list));
	if (!command_list)
		return (FAILURE);
	command_list->type = -1;
	command_list->commands = NULL;
	command_list->pipeline_token_list = NULL;
	command_list->subshell_token_list = NULL;

	pipeline_token = (*pipeline_node)->token_list_head;
	(*pipeline_node)->command_list = NULL;//for lstadd_back;

	while (pipeline_token)
	{
		token_elem = pipeline_token->content;

		if (token_elem->type == e_subshell_start)
		{
			// ( )を取り除いてsubshell_token_listに入れる
			// (hoge), (hoge)>out
			depth = token_elem->depth;
			// 今のdepthよりも小さなdepthの|まで実行
			while (pipeline_token && !(token_elem->type == e_ope_pipe && token_elem->depth == depth + 1))
			{
				next = pipeline_token->next;
				if (is_tokentype_subshell(token_elem->type) && token_elem->depth == depth)
					ft_lstdelone(pipeline_token, free_token_elem); // delete "(", ")"
				else
				{
					popped_token = pipeline_token;
					popped_token->next = NULL;
					ft_lstadd_back(&command_list->subshell_token_list, popped_token);
				}
				pipeline_token = next;
				if (pipeline_token)
					token_elem = pipeline_token->content;
			}
		}
		else if (token_elem->type != e_ope_pipe)
		{
			while (pipeline_token && token_elem->type != e_ope_pipe)
			{
				next = pipeline_token->next;
				// command_list, command_list->pipeline_tolen_list+=pipeline_token;
				popped_token = pipeline_token;
				popped_token->next = NULL;
				ft_lstadd_back(&command_list->pipeline_token_list, popped_token);
				pipeline_token = next;
				if (pipeline_token)
					token_elem = pipeline_token->content;
			}
		}
		// pipe or end
		new_pipeline = ft_lstnew(command_list);
		if (!new_pipeline)
			return (FAILURE); //TODO
		ft_lstadd_back(&(*pipeline_node)->pipeline, new_pipeline);

		command_list = (t_command_list *)malloc(sizeof(t_command_list));
		if (!command_list)
			return (FAILURE);
		command_list->type = -1;
		command_list->commands = NULL;
		command_list->pipeline_token_list = NULL;
		command_list->subshell_token_list = NULL;

		prev = pipeline_token;
		if (pipeline_token)
			pipeline_token = pipeline_token->next;
		ft_lstdelone(prev, free_token_elem); //delete pipe node
	}
	return (SUCCESS);
}


/*
// commandlistにsubshellは1つのみ
int create_command_list_from_pipeline_node(t_exec_list **pipeline_node)
{
	t_list			*tokenlist;

	t_list			*command_node;
	t_list			*popped_node;

	t_list			*next;

	t_token_elem	*token;
	ssize_t			depth;
	t_commands_info	*commands_info;


	if (!pipeline_node || !*pipeline_node || !(*pipeline_node)->token_list_head)
		return (FAILURE);

	// command_list
	tokenlist = (*pipeline_node)->token_list_head;
	(*pipeline_node)->command_list_head = NULL; //for lstadd_back;
	errno = 0;
	commands_info = (t_commands_info *)malloc(sizeof(t_commands_info));
	if (!commands_info)
		return (perror_ret_int("malloc", FAILURE));
	commands_info->type = -1;
	commands_info->commands = NULL;

	while (tokenlist)
	{
		next = tokenlist->next;
		token = tokenlist->content;
		if (token->type == e_ope_pipe)
		{
			command_node = ft_lstnew(commands_info);
			if (!command_node)
				return (perror_ret_int("malloc", FAILURE)); //TODO: free?
			ft_lstadd_back(&(*pipeline_node)->command_list_head, command_node);

			// 次のために作成
			commands_info = (t_commands_info *)malloc(sizeof(t_commands_info));
			if (!commands_info)
				return (perror_ret_int("malloc", FAILURE));

			ft_lstdelone(tokenlist, free_token_elem); // delete "|"
		}
		else if (token->type == e_subshell_start)
		{
			// ( hoge huga (piyo) )
			//   ^^^^^^^^^^^^^^^^ command_info->subshell
			commands_info->type = e_subshell;
			depth = token->depth;
			while (token->depth >= depth)
			{
				next = tokenlist->next;
				if (!(is_tokentype_subshell(token->type) && token->depth == depth))
				{
					popped_node = tokenlist;
					popped_node->next = NULL;
					ft_lstadd_back(&commands_info->subshell_head, popped_node);

				}
				if (is_tokentype_subshell(token->type) && token->depth == depth)
					ft_lstdelone(tokenlist, free_token_elem); // delete "(", ")"
				if (!next)
				{
					command_node = ft_lstnew(commands_info);
					if (!command_node)
						return (perror_ret_int("malloc", FAILURE)); //TODO: free?
					ft_lstadd_back(&(*pipeline_node)->command_list_head, command_node);
					break ;
				}
				tokenlist = next;
				token = tokenlist->content;
			}

		}
		else
		{
			commands_info->type = e_commands;
			while (token->type != e_ope_pipe && is_tokentype_subshell(token->type))
			{
				next = tokenlist->next;
				popped_node = tokenlist;
				popped_node->next = NULL;
				ft_lstadd_back(&commands_info->subshell_head, popped_node);

				if (!next)
				{
					command_node = ft_lstnew(commands_info);
					if (!command_node)
						return (perror_ret_int("malloc", FAILURE)); //TODO: free?
					ft_lstadd_back(&(*pipeline_node)->command_list_head, command_node);
					break ;
				}
				tokenlist = next;
			}

		}
		tokenlist = next;
	}

	// redirect_info;

	return (SUCCESS);
}
*/

// (cd /bin && ls) > out どうする？

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
		return (FAILURE);
	// print
	debug_print_exec_list(exec_list_head, "operator_list");




	// tokenlist->tree
	// return
	return (SUCCESS);
}
































