/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 20:14:39 by takira            #+#    #+#             */
/*   Updated: 2023/02/01 19:23:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

char **create_minishell_envp(t_list *envlist_head)
{
	char		**minishell_envp;
	size_t		array_size;
	size_t		idx;
	t_env_elem	*env_elem;
	t_list		*node;

	dprintf(STDERR_FILENO, "#[DEBUG] minishell envp 1\n");

	array_size = ft_lstsize(envlist_head);
	minishell_envp = (char **) ft_calloc(sizeof(char *), array_size + 1);
	if (!minishell_envp)
		return (perror_ret_nullptr("malloc"));

	dprintf(STDERR_FILENO, "#[DEBUG] minishell envp 2\n");

	node = envlist_head;
	idx = 0;
	while (node && idx < array_size)
	{
		dprintf(STDERR_FILENO, "#[DEBUG] minishell envp 3, idx:%zu\n", idx);

		env_elem = node->content;
		dprintf(STDERR_FILENO, "#[DEBUG] minishell envp 3-1, key:%s\n", env_elem->key);
		minishell_envp[idx] = concat_dst_to_src(&minishell_envp[idx], env_elem->key);
		if (!minishell_envp[idx])
			return (NULL); //TODO: all free
		dprintf(STDERR_FILENO, "#[DEBUG] minishell envp 3-2\n");

		minishell_envp[idx] = concat_dst_to_src(&minishell_envp[idx], "=");
		if (!minishell_envp[idx])
			return (NULL); //TODO: all free
		dprintf(STDERR_FILENO, "#[DEBUG] minishell envp 3-3, value:%s\n", env_elem->value);
		minishell_envp[idx] = concat_dst_to_src(&minishell_envp[idx], env_elem->value);
		if (!minishell_envp[idx])
			return (NULL); //TODO: all free
		node = node->next;
		idx++;
	}
	dprintf(STDERR_FILENO, "#[DEBUG] minishell envp 4\n");

	return (minishell_envp);
}
