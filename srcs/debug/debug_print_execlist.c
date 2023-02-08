/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print_execlist.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 20:24:52 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 20:47:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print_exec_nodetype(t_exec_list *node)
{
	char		*type_str;
	const char	*type[] = {";", "|", "||", "&&", "(", ")", \
	"<", ">", ">>", "<<", "file", "eof", "word", "init", NULL};

	if (node->node_kind == e_node_head)
		type_str = "head";
	else
		type_str = (char *)type[node->token_type];
	ft_dprintf(STDERR_FILENO, "  v v v \n", type_str);
	ft_dprintf(STDERR_FILENO, "[exec node:%s]\n", type_str);
	ft_dprintf(STDERR_FILENO, "  v v v \n", type_str);
}

static void	print_pipeline(t_exec_list *node)
{
	t_list_bdi	*pipeline;

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

void	debug_print_exec_list(t_exec_list *node, char *str)
{
	const char	*type[] = {";", "|", "||", "&&", "(", ")", \
	"<", ">", ">>", "<<", "file", "eof", "word", "init", NULL};

	ft_dprintf(STDERR_FILENO, "\n[#DEBUG print] %s %s", str, "\n");
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
			ft_dprintf(STDERR_FILENO, \
			"    %s operator\n", type[node->token_type]);
		else
			print_pipeline(node);
		node = node->next;
	}
	ft_dprintf(STDERR_FILENO, "    v\n  [end]\n");
}
