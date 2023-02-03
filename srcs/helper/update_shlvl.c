/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 21:56:24 by takira            #+#    #+#             */
/*   Updated: 2023/02/04 08:52:09 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_new_shlvl_num(int now_shlvl_num, bool is_atoi_success);
static int	is_new_shlvl_over_flow(int now_shlvl_num, bool is_atoi_success);

/* return value		: SUCCESS or FAILURE of the process. */
/* new SHLVL string	: update *now_shlvl_str passed in the argument **now_shlvl_str. */
int update_shlvl(char **now_shlvl_str)
{
	int		now_shlvl_num;
	int 	new_shlvl_num;
	char	*new_shlvl_str;
	bool	is_atoi_success;

	if (!now_shlvl_str)
		return (FAILURE);
	now_shlvl_num = ft_atoi(*now_shlvl_str, &is_atoi_success);
	*now_shlvl_str = free_1d_alloc(*now_shlvl_str);
	if (is_atoi_success && now_shlvl_num == 999)
	{
		*now_shlvl_str = ft_strdup("");
		return (SUCCESS);
	}
	new_shlvl_num = get_new_shlvl_num(now_shlvl_num, is_atoi_success);
	if (1000 < new_shlvl_num)
	{
		ft_dprintf(STDERR_FILENO, "minishell: warning: shell level (%d) too high, resetting to 1", new_shlvl_num);
		new_shlvl_num = 1;
	}
	new_shlvl_str = ft_itoa(new_shlvl_num);
	if (!new_shlvl_str)
		return (perror_ret_int("malloc", FAILURE));
	*now_shlvl_str = new_shlvl_str;

	ft_dprintf(STDERR_FILENO, "[#DEBUG shlvl] now:%d, newnum:%d, str:%s\n", now_shlvl_num, new_shlvl_num, new_shlvl_str);
	return (SUCCESS);
}

static int	get_new_shlvl_num(int now_shlvl_num, bool is_atoi_success)
{
	if (!is_atoi_success)
		return (1);
	if (now_shlvl_num == INT_MAX)
		return (0);
	return (now_shlvl_num + 1);
}

int	add_initial_shlvl(t_list **env_list_head)
{
	t_env_elem		*elem;

	if (!env_list_head)
		return (FAILURE);
	errno = 0;
	elem = (t_env_elem *)malloc(sizeof(t_env_elem));
	if (!elem)
		return (perror_ret_int("malloc", FAILURE));
	elem->key = ft_strdup("SHLVL");
	elem->value = ft_strdup("1");
	elem->not_print = 0;
	if (!elem->key || !elem->value)
	{
		free_env_elem(elem);
		elem = NULL;
		return (perror_ret_int("malloc", FAILURE));
	}
	return (SUCCESS);
}
