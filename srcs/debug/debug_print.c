/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:21:33 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 11:20:27 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print_exec_list(t_exec_list *node, char *str)
{
	const char	*type[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", "file", "eof", "word", "init", NULL};
	t_list_bdi	*pipeline;

	ft_dprintf(STDERR_FILENO, "\n[#DEBUG print] %s %s", str ? str : "", "\n");
	if (!node)
	{
		ft_dprintf(STDERR_FILENO, "null\n");
		return ;
	}
	while (node)
	{
		if (node->node_kind == e_node_head)
			ft_dprintf(STDERR_FILENO, "  [head]\n    v\n");
		if (node->node_kind == e_node_semicolon
		|| node->node_kind == e_node_and
		|| node->node_kind == e_node_or)
			ft_dprintf(STDERR_FILENO, "    %s operator\n", type[node->token_type]);
		else
		{
			if (node->token_list_head)
			{
				ft_dprintf(STDERR_FILENO, "  [pipeline] ");
				ft_dprintf(STDERR_FILENO, "%-10s:", "token_list");
				debug_print_tokens(node->token_list_head, NULL);
			}
			pipeline = node->pipeline_commands;
			while (pipeline)
			{
				debug_print_command_info(pipeline->content);
				pipeline = pipeline->next;
				if (pipeline)
					ft_dprintf(STDERR_FILENO, "      v pipe\n");
			}
		}
		node = node->next;
	}
	ft_dprintf(STDERR_FILENO, "    v\n  [end]\n");
}


void	debug_print_exec_nodetype(t_exec_list *node)
{
	const char	*type[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", "file", "eof", "word", "init", NULL};
	char 		*type_str;

	if (node->node_kind == e_node_head)
		type_str = "head";
	else
		type_str = (char *)type[node->token_type];
	ft_dprintf(STDERR_FILENO, "  v v v \n", type_str);
	ft_dprintf(STDERR_FILENO, "[exec node:%s]\n", type_str);
	ft_dprintf(STDERR_FILENO, "  v v v \n", type_str);
}


void	debug_print_tree(t_exec_list *root, char *str)
{
	const char	*type[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", "file", "eof", "word", "init", NULL};
	t_exec_list	*node;
	t_exec_list	*left_node;

	if (str)
		ft_dprintf(STDERR_FILENO, "#%-15s\n", str);
	if (!root)
	{
		ft_dprintf(STDERR_FILENO, "tree node is null\n");
		return ;
	}
	ft_dprintf(STDERR_FILENO, "[root]\n  |\n");
	node = root;
	while (node)
	{
		if (node->node_kind == e_node_pipeline)
		{
			ft_dprintf(STDERR_FILENO, "[pipeline_commands] :");
			debug_print_tokens(node->token_list_head, NULL);
		}
		else
		{
			// prev = pipeline_commands
			left_node = node->prev;
			ft_dprintf(STDERR_FILENO, "[pipeline_commands] :");
			debug_print_tokens(left_node->token_list_head, NULL);

			// print operator node
			if (node->node_kind != e_node_head)
				ft_dprintf(STDERR_FILENO, "[%s]\n  |\n", type[node->token_type]);
		}
		node = node->next;
	}
}

void debug_print_2d_arr(char **arr, char *str)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	if (str)
		ft_dprintf(STDERR_FILENO, "#%-15s:", str);
	ft_dprintf(STDERR_FILENO, "{");
	while (arr[i])
	{
		ft_dprintf(STDERR_FILENO, "\"%s\"", arr[i++]);
		if (arr[i])
			ft_dprintf(STDERR_FILENO, ", ");
		else
		{
			if (i > 0)
				ft_dprintf(STDERR_FILENO, ", ");
			ft_dprintf(STDERR_FILENO, "NULL}\n");
		}
	}
}

void	debug_print_redirect_info(t_list_bdi *head, char *str)
{
	t_list_bdi		*node;
	t_redirect_info	*info;
	const char	*type[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", "f", "e", "w", "i", NULL};

	if (str)
		ft_dprintf(STDERR_FILENO, "#%-15s:", str);
	node = head;
	while (node)
	{
		info = node->content;
		ft_dprintf(STDERR_FILENO, "%s[", type[info->io_type]);
		if (info->io_type == e_heredoc)
			ft_dprintf(STDERR_FILENO, "heredoc eof:%s, file:%s]%s", info->heredoc_eof, info->filename, info->is_expansion ? "y" : "n");
		else
			ft_dprintf(STDERR_FILENO, "filename:%s]", info->filename);
		node = node->next;
		if (node)
			ft_dprintf(STDERR_FILENO, ", ");
		else
			ft_dprintf(STDERR_FILENO, "\n");
	}
}

void	print_wildcard_valid_list(t_token_elem *token)
{
	size_t	len = ft_strlen_ns(token->word);
	size_t	idx = 0;

	if (!token->wildcard_valid_flag)
		return ;
	ft_dprintf(STDERR_FILENO, "(");
	while (token->wildcard_valid_flag && idx < len)
	{
		ft_dprintf(STDERR_FILENO, "%d", token->wildcard_valid_flag[idx]);
		idx++;
		if (idx < len)
			ft_dprintf(STDERR_FILENO, ",");
	}
	ft_dprintf(STDERR_FILENO, ")");
}

void	debug_print_tokens(t_list_bdi *head, char *str)
{
	t_list_bdi		*node;
	t_token_elem	*token;
	const char	*type[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", "f", "e", "w", "i", NULL};

	if (str)
		ft_dprintf(STDERR_FILENO, "#%-15s:", str);
	node = head;
	while (node)
	{
//		printf("\nnode:%p, next:%p\n", node, node->next);
		token = node->content;

		ft_dprintf(STDERR_FILENO, "[%s]", token->word);
		ft_dprintf(STDERR_FILENO, "%s", type[token->type]);
		if (token->quote_chr == CHR_DOUBLE_QUOTE)
			ft_dprintf(STDERR_FILENO, "\"");
		if (token->quote_chr == CHR_SINGLE_QUOTE)
			ft_dprintf(STDERR_FILENO, "\'");
//		if (!token->is_wildcard_expandable)
//			ft_dprintf(STDERR_FILENO, "*"); //expand wildcard
		print_wildcard_valid_list(token);
		ft_dprintf(STDERR_FILENO, "%d", token->subshell_depth);
		if (token->is_connect_to_next_word && node->next)
			ft_dprintf(STDERR_FILENO, "=");
		if (!token->is_connect_to_next_word && node->next)
			ft_dprintf(STDERR_FILENO, ",");
		node= node->next;
	}
	ft_dprintf(STDERR_FILENO, "\n");
}

void	debug_print_command_info(t_command_info *command_info)
{
	bool	is_first_print = true;

	ft_dprintf(STDERR_FILENO, "  [pipeline] ");
	/* subshell */
	if (command_info && command_info->subshell_token_list)
	{
		if (!is_first_print)
			ft_dprintf(STDERR_FILENO, "             ");
		ft_dprintf(STDERR_FILENO, "%-18s:", "subshell(token)");
		debug_print_tokens(command_info->subshell_token_list, NULL);
		is_first_print = false;
	}

	/* commands */
	if (command_info && command_info->pipeline_token_list)
	{
		if (!is_first_print)
			ft_dprintf(STDERR_FILENO, "             ");
		ft_dprintf(STDERR_FILENO, "%-18s:", "commands(token)");
		debug_print_tokens(command_info->pipeline_token_list, NULL);
		is_first_print = false;
	}

	if (command_info && command_info->commands)
	{
		if (!is_first_print)
			ft_dprintf(STDERR_FILENO, "             ");
		ft_dprintf(STDERR_FILENO, "%-18s:", "commands(char**)");
		debug_print_2d_arr(command_info->commands, NULL);
		is_first_print = false;
	}

	/* redirection */
	if (command_info && command_info->redirect_list)
	{
		if (!is_first_print)
			ft_dprintf(STDERR_FILENO, "             ");
		ft_dprintf(STDERR_FILENO, "%-18s:", "redirect(token)");
		debug_print_redirect_info(command_info->redirect_list, NULL);
		is_first_print = false;
	}
}

void	debug_print_wildcard_valid_list(int *list, size_t len)
{
	size_t	idx = 0;

	printf("(");
	while (idx < len)
	{
		printf("%d", list[idx]);
		idx++;
		if (idx < len)
			printf(",");
	}
	printf(")\n");
}

void	debug_print_env(t_list *envlist)
{
	t_env_elem	*env_elem;

//	printf("\n[@@ degug print]envlist\n");
//	printf("envlist pointer:%p\n", envlist);
	if (!envlist)
	{
		printf("(null)\n");
		return ;
	}
	while (envlist)
	{
		env_elem = envlist->content;
//		printf("envelem pointer:%p\n", env_elem);
		if (!env_elem)
			printf("content: (null)\n");
		else
			printf("key: %s, value:%s\n", env_elem->key, env_elem->value);
		envlist = envlist->next;
	}
}

void	print_dp_array(int **array, const char *wild, const char *target)
{
	size_t			row;
	size_t			col;
	const size_t	wild_len = ft_strlen_ns(wild);
	const size_t	str_len = ft_strlen_ns(target);

	if (!array)
		return ;
	row = 0;
	while (row < wild_len + 2)
	{
		col = 0;
		while (col < str_len + 2)
		{
			if ((row == 0 && col == 0) || (row == 1 && col == 0) || (row == 0 && col == 1))
				ft_printf("- ");
			else if (row == 0)
				ft_printf("%c ", target[col - 2]);
			else if (col == 0)
				ft_printf("%c ", wild[row - 2]);
			else
				ft_printf("%d ", array[row - 1][col - 1]);
			col++;
		}
		row++;
		ft_printf("\n");
	}
	ft_printf("\n");
}

