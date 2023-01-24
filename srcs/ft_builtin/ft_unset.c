/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 21:07:36 by wchen             #+#    #+#             */
/*   Updated: 2023/01/24 22:27:33 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_elem(t_list *pre_node, t_list *cur_node)
{
	t_list	*temp_node;

	if (pre_node == NULL)
	{
		temp_node = cur_node;
		cur_node = cur_node->next;
		ft_lstdelone(temp_node, free_env_elem);
	}
	else
	{
		pre_node->next = cur_node->next;
		ft_lstdelone(cur_node, free_env_elem);
	}
	return (EXIT_SUCCESS);
}

static int	unset_env(t_info *info, char *cmd)
{
	t_list	*pre_node;
	t_list	*env_node;

	pre_node = NULL;
	env_node = info->envlist_head;
	while (env_node != NULL)
	{
		if(get_value_from_key(env_node->content, cmd) != NULL)
			return (unset_elem(pre_node, env_node));
		pre_node = env_node;
		env_node = env_node->next;
	}
	return (EXIT_SUCCESS);
}

int	 ft_unset(t_info *info, char **cmds)
{
	int	exit_status;

	(void)info;
	exit_status = EXIT_SUCCESS;
	cmds ++;
	while (*cmds != NULL)
	{
		if (judge_chr_key(*cmds) == FAILURE)
		{
			ft_dprintf(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n",*cmds);
			exit_status += EXIT_FAILURE;
		}
		else
			exit_status += unset_env(info, *cmds);
		cmds ++;
	}
	if (exit_status > 0)
		return (EXIT_FAILURE);
	return (exit_status);
}
