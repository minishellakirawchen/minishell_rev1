/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   judge_fork_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 20:43:38 by takira            #+#    #+#             */
/*   Updated: 2023/01/28 20:43:38 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_execution.h"

bool	is_fork_failure(pid_t pid)
{
	return (pid < 0);
}

bool	is_child_process(pid_t pid)
{
	return (pid == 0);
}

bool	is_parent_process(pid_t pid)
{
	return (pid > 0);
}
