/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envlist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:13:19 by takira            #+#    #+#             */
/*   Updated: 2023/02/03 21:34:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* prototype declaration*/
static t_env_elem	*get_env_elem(const char *environ_i, bool *is_shlvl);
int	add_initial_shlvl(t_list **env_list_head);

/* function */
t_list	*get_envlist(void)
{
	extern char	**environ;
	t_list		*env_list_head;
	t_list		*new_node;
	t_env_elem	*env_elem;
	size_t		idx;
	bool		is_shlvl_exist;

	idx = 0;
	if (!environ)
		return (NULL);
//	debug_print_2d_arr(environ, "environ");
	is_shlvl_exist = false;
	env_list_head = NULL;
	errno = 0;
	while (environ[idx])
	{
		env_elem = get_env_elem(environ[idx], &is_shlvl_exist);
		if (!env_elem)
		{
			ft_lstclear(&env_list_head, free_env_elem);
			return (NULL);
		}
		new_node = ft_lstnew(env_elem);
		if (!new_node)
		{
			free(env_elem);
			ft_lstclear(&env_list_head, free_env_elem);
			return (NULL);
		}
		ft_lstadd_back(&env_list_head, new_node);
		idx++;
	}
	if (!is_shlvl_exist && add_initial_shlvl(&env_list_head) == FAILURE)
	{
		ft_lstclear(&env_list_head, free_env_elem);
		return (NULL);
	}
	return (env_list_head);
}

// '='idx
//  v  v len
// 0123
// A=B
//   ^ len-idx-1

// A= :OK
// A  :NG

// SHLVL
// 100 -> 101
// 255 -> 256 -> 257
// 998 -> 999 -> SHLVL="" -> 1

// not display warning
// "a" -> "1"
// -1 -> 1
// -100 -> 1
// 2147483647 -> 0

// display warning
// SHLVL="1000" -> "1"	//bash: warning: shell level (1001) too high, resetting to 1
// SHLVL="32766" -> "1"	//bash: warning: shell level (32767) too high, resetting to 1
// 	2147483646 -> "1" (	2147483647) too hight

static int update_shlvl(char **value)
{
	int		now_shlvl_num;
	int 	new_shlvl_num;
	char	*new_shlvl_str;
	bool	is_atoi_success;

	if (!value)
		return (FAILURE);
	now_shlvl_num = ft_atoi(*value, &is_atoi_success);
	*value = free_1d_alloc(*value);
	if (is_atoi_success && now_shlvl_num == 999)
	{
		*value = ft_strdup("");
		return (SUCCESS);
	}
	if (!is_atoi_success)
		new_shlvl_num = 1;
	else
	{
		if (now_shlvl_num <= 0 || (1000 <= now_shlvl_num && now_shlvl_num < INT_MAX))
			new_shlvl_num = 1;
		else if (now_shlvl_num == INT_MAX)
			new_shlvl_num = 0;
		else
			new_shlvl_num = now_shlvl_num + 1;
		if (1000 <= now_shlvl_num)
			ft_dprintf(STDERR_FILENO, "minishell: warning: shell level (%d) too high, resetting to 1", now_shlvl_num + 1);
	}
	new_shlvl_str = ft_itoa(new_shlvl_num);
	if (!new_shlvl_str)
		return (perror_ret_int("malloc", FAILURE));
	*value = new_shlvl_str;
	printf("[shlvl] now:%d, newnum:%d, str:%s\n", now_shlvl_num, new_shlvl_num, new_shlvl_str);
	return (SUCCESS);
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


static t_env_elem	*get_env_elem(const char *environ_i, bool *is_shlvl)
{
	t_env_elem		*elem;
	size_t			idx;
	const size_t	len = ft_strlen_ns(environ_i);

	if (!environ_i)
		return (NULL);//env=NULL
	idx = 0;
	while (environ_i[idx] && environ_i[idx] != '=')
		idx++;
	if (environ_i[idx] == '\0')
		return (NULL);
	errno = 0;
	elem = (t_env_elem *)malloc(sizeof(t_env_elem));
	if (!elem)
		return (perror_ret_nullptr("malloc"));
	elem->key = ft_substr(environ_i, 0, idx);
	elem->value = ft_substr(environ_i, idx + 1, len - idx - 1);
	elem->not_print = 0;
	if (!elem->key || !elem->value)
	{
		free_env_elem(elem);
		elem = NULL;
		return (perror_ret_nullptr("malloc"));
	}
	if (is_same_str(elem->key, "SHLVL"))
	{
		*is_shlvl = true;
		if (update_shlvl(&elem->value) == FAILURE)
		{
			free_env_elem(elem);
			return (perror_ret_nullptr("malloc"));
		}
	}
	return (elem);
}
