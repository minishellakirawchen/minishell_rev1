/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 21:23:30 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 14:17:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

//int	execute_redirect(t_command_info *command_info, t_list *envlist)
//{
//	if (!command_info)
//		return (FAILURE);
//	if (!command_info->redirect_list)
//		return (SUCCESS);
//
//	// heredocがredirect in/outより優先される
//	if (execute_heredoc(command_info, envlist) == FAILURE)
//		return (FAILURE);
//
//	// openfileはheredocの後
//	if (open_file_for_redirect(command_info) == FAILURE)
//		return (FAILURE);
//
//	return (SUCCESS);
//}

