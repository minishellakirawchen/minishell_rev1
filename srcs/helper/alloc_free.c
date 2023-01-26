/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:00:09 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 14:47:18 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* use in debug print to display node kind */
char *get_node_char(t_node_kind type)
{
	if (type == e_node_head)
		return ("node_head");
	if (type == e_node_operator)
		return ("node_operator");
	if (type == e_node_subshell)
		return ("node_subshell");
	if (type == e_node_pipeline)
		return ("node_pipeline");
	if (type == e_node_commands)
		return ("node_commands");
	return ("node_init");}

void	clear_exec_list(t_exec_list **exec_list)
{
	t_exec_list	*next;

	if (!exec_list || !*exec_list)
		return ;
	while (*exec_list)
	{
		next = (*exec_list)->next;
		ft_lstclear_bdi(&(*exec_list)->token_list_head, free_token_elem);
		ft_lstclear_bdi(&(*exec_list)->pipeline_commands, free_command_info);
		free(*exec_list);
		*exec_list = next;
	}
}

void	**free_2d_alloc(void **alloc)
{
	size_t	i;

	if (!alloc)
		return (NULL);
	i = 0;
	while (alloc[i])
	{
		free_1d_alloc(alloc[i]);
		i++;
	}
	free(alloc);
	return (NULL);
}


void	*free_1d_alloc(void *alloc)
{
	free(alloc);
	return (NULL);
}

void	*free_info(t_info **info)
{
	if (!info || !*info)
		return (NULL);

	ft_lstclear(&(*info)->envlist_head, free_env_elem);
	(*info)->envlist_head = NULL;

	clear_input_info(info);

	free(*info);
	*info = NULL;
	return (NULL);
}

void	free_env_elem(void *content)
{
	t_env_elem	*elem;

	if (!content)
		return ;
	elem = content;
	elem->key = free_1d_alloc(elem->key);
	elem->value = free_1d_alloc(elem->value);
	free_1d_alloc(elem);
}

void	free_command_info(void *content)
{
	t_command_info	*elem;

	if (!content)
		return ;
	elem = content;
	elem->commands = (char **)free_2d_alloc((void **)elem->commands);
	ft_lstclear_bdi(&(elem->pipeline_token_list), free_token_elem);
	ft_lstclear_bdi(&(elem->subshell_token_list), free_token_elem);
	ft_lstclear_bdi(&(elem->redirect_list), free_redirect_info);
	free_1d_alloc(elem);
}

void	free_redirect_info(void *content)
{
	t_redirect_info	*redirect_info;

	if (!content)
		return ;
	redirect_info = content;
	free_1d_alloc(redirect_info->file);
	free_1d_alloc(redirect_info->heredoc_eof);
	free_1d_alloc(redirect_info);
}

void	free_token_elem(void *content)
{
	t_token_elem	*elem;

	if (!content)
		return ;
	elem = content;
//	printf("#debug free_token:%s\n", elem->word);
	elem->word = free_1d_alloc(elem->word);
	free_1d_alloc(elem);
}


