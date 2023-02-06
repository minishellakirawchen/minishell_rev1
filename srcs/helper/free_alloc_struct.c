/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_alloc_struct.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 15:10:50 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 13:39:32 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	errno = 0;
	if (redirect_info->io_type == e_heredoc)
		if (redirect_info->filename && unlink(redirect_info->filename) < 0)
			perror("unlink");
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
	elem->word = free_1d_alloc(elem->word);
	elem->wildcard_valid_flag = free_1d_alloc(elem->wildcard_valid_flag);
	free_1d_alloc(elem);
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
