/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:00:09 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 10:38:45 by takira           ###   ########.fr       */
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
	ft_lstclear_bdi(&(elem->redirect_list), free_redirect_info);
	ft_lstclear_bdi(&(elem->pipeline_token_list), free_token_elem);
	ft_lstclear_bdi(&(elem->subshell_token_list), free_token_elem);
	free_1d_alloc(elem);
}

void	free_redirect_info(void *content)
{
	t_redirect_info	*redirect_info;

	if (!content)
		return ;
	redirect_info = content;
	if (redirect_info->io_type == e_heredoc)
		if (unlink(redirect_info->filename) < 0)
			perror_and_return_int("unlink", FAILURE);
	free_1d_alloc(redirect_info->filename);
	free_1d_alloc(redirect_info->heredoc_eof);
	ft_lstclear_bdi(&redirect_info->token_list, free_token_elem);
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


