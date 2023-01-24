/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:00:09 by takira            #+#    #+#             */
/*   Updated: 2023/01/24 14:15:58 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	t_exec_list	*node;

	if (!exec_list || !*exec_list)
		return ;
	node = *exec_list;
	while (node)
	{   printf("free_exec node:%s\n", get_node_char(node->node_kind));
		next = node->next;
		ft_lstclear(&node->token_list_head, free_token_elem);
		ft_lstclear(&node->pipeline, free_command_list_elem);
		free(node);
		node = next;
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

void	free_command_list_elem(void *content)
{
	t_command_list	*elem;

	if (!content)
		return ;
	elem = content;
	elem->commands = (char **)free_2d_alloc((void **)elem->commands);
	ft_lstclear(&elem->pipeline_token_list, free_token_elem);
	ft_lstclear(&elem->subshell_token_list, free_token_elem);
	
	//TODO: delete redirect_list
}

void	free_token_elem(void *content)
{
	t_token_elem	*elem;

	if (!content)
		return ;
	elem = content;
	elem->word = free_1d_alloc(elem->word);
	free_1d_alloc(elem);
}


