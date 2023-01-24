/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:21:33 by takira            #+#    #+#             */
/*   Updated: 2023/01/24 15:42:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print_exec_list(t_exec_list *node, char *str)
{
	const char	*type[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", "file", "eof", "word", "init", NULL};
	t_command_list	*command_list;
	t_list			*pipeline;

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
		if (node->node_kind == e_node_operator)
			ft_dprintf(STDERR_FILENO, "    %s operator\n", type[node->token_type]);
		else
		{
			if (node->token_list_head)
			{
				ft_dprintf(STDERR_FILENO, "  [pipeline] ");
				ft_dprintf(STDERR_FILENO, "%-10s:", "token_list");
				debug_print_token_word(node->token_list_head, NULL);
			}
			pipeline = node->pipeline_commands;
			while (pipeline)
			{
				command_list = pipeline->content;
				ft_dprintf(STDERR_FILENO, "  [pipeline] ");
				ft_dprintf(STDERR_FILENO, "%-10s:", "subshell");
				if (command_list && command_list->subshell_token_list)
					debug_print_token_word(command_list->subshell_token_list, NULL);
				else
					ft_dprintf(STDERR_FILENO, "\n");
				ft_dprintf(STDERR_FILENO, "             %-10s:", "commands");
				if (command_list && command_list->pipeline_token_list)
					debug_print_token_word(command_list->pipeline_token_list, NULL);
				else
					ft_dprintf(STDERR_FILENO, "\n");
				pipeline = pipeline->next;
				if (pipeline)
					ft_dprintf(STDERR_FILENO, "      v pipe\n");
			}
		}
		node = node->next;
	}
	ft_dprintf(STDERR_FILENO, "    v\n  [tail]\n");
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
			debug_print_token_word(node->token_list_head, NULL);
		}
		else
		{
			// prev = pipeline_commands
			left_node = node->prev;
			ft_dprintf(STDERR_FILENO, "[pipeline_commands] :");
			debug_print_token_word(left_node->token_list_head, NULL);

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
	}
	ft_dprintf(STDERR_FILENO, "}\n");
}

void	debug_print_token_word(t_list *head, char *str)
{
	t_list			*node;
	t_token_elem	*token;
	const char	*type[] = {";", "|", "||", "&&", "(", ")", "<", ">", ">>", "<<", "f", "e", "w", "i", NULL};

	if (str)
		ft_dprintf(STDERR_FILENO, "#%-15s:", str);
	node = head;
	while (node)
	{
		token = node->content;

		ft_dprintf(STDERR_FILENO, "[%s]", token->word);
		ft_dprintf(STDERR_FILENO, "%s", type[token->type]);
		ft_dprintf(STDERR_FILENO, "%d", token->depth);
		ft_dprintf(STDERR_FILENO, "%c", token->is_quoted ? token->word[0] : '\0');

		if (token->is_connect_to_next_word && node->next)
			ft_dprintf(STDERR_FILENO, "=");
		if (!token->is_connect_to_next_word && node->next)
			ft_dprintf(STDERR_FILENO, ",");
		node= node->next;
	}
	ft_dprintf(STDERR_FILENO, "\n");
}
