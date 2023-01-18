/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:21:33 by takira            #+#    #+#             */
/*   Updated: 2023/01/18 20:44:34 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void debug_print_2d_arr(char **arr, char *str)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	if (str)
		ft_dprintf(STDERR_FILENO, "%s:", str);
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
	t_list			*ptr;
	t_token_elem	*elem;

	if (str)
		ft_dprintf(STDERR_FILENO, "# %s", str);
	ft_dprintf(STDERR_FILENO, ":", str);
	ptr = head;
	while (ptr)
	{
		elem = ptr->content;
		ft_dprintf(STDERR_FILENO, "[%s]", elem->word);
		if (elem->is_connect_to_next_word && ptr->next)
			ft_dprintf(STDERR_FILENO, "=");
		if (!elem->is_connect_to_next_word && ptr->next)
			ft_dprintf(STDERR_FILENO, ",");
		ptr = ptr->next;
	}
	ft_dprintf(STDERR_FILENO, "\n\n");
}

/*
void	debug_print_stack(t_tree *root, char *str)
{
	t_tree		*tree;

	ft_dprintf(STDERR_FILENO, "\n");
	if (str)
		ft_dprintf(STDERR_FILENO, "#DEBUG[print_stack : %s]\n", str);
	tree = root;
	while (tree)
	{
		if (tree->exe_type == E_NODE_ROOT)
		{
			ft_dprintf(STDERR_FILENO, " [root]\n");
			ft_dprintf(STDERR_FILENO, "  |    \n");
		}
		else if (tree->exe_type == E_NODE_PIPE)
		{
			ft_dprintf(STDERR_FILENO, " [pipe]\n");
			ft_dprintf(STDERR_FILENO, "  |    \n");
		}
		else if (tree->exe_type == E_NODE_NO_PIPE)
		{
			ft_dprintf(STDERR_FILENO, " [shell]\n");
			ft_dprintf(STDERR_FILENO, "  |    \n");
		}
		else if (tree->exe_type == E_LEAF_COMMAND)
		{
			ft_dprintf(STDERR_FILENO, " [cmd]--");
			debug_print_2d_arr(tree->cmds, NULL);
		}
		tree = tree->next;
	}
	ft_dprintf(STDERR_FILENO, "\n");
}
*/