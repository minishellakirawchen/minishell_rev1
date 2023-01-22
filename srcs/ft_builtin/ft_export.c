/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 21:07:02 by wchen             #+#    #+#             */
/*   Updated: 2023/01/23 00:05:29 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	define_key_value(char *cmd, char **key, char **value,
	int *skip_flag)
{
	char	*key_sign;
	int		exit_status;
	ssize_t	equal_index;
	ssize_t	cmd_len;

	exit_status = EXIT_SUCCESS;
	equal_index = 0;
	key_sign = ft_strchr(cmd, '=');
	//'='を見つからないときに、何もせずに、e_return(skip_sign)を返す
	if (key_sign == NULL)
	{
		*skip_flag = 1;
		return (EXIT_SUCCESS);
	}
	*skip_flag = 0;
	cmd_len = ft_strlen(cmd);
	while (cmd[equal_index] != '=')
		equal_index++;
	*key = ft_substr(cmd, 0, equal_index);
	*value = ft_substr(cmd, equal_index + 1, (cmd_len - (equal_index + 1)));
	if (!key || !value)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	return (exit_status);
}

static t_key_type	judge_key(char *key)
{
	ssize_t	i;
	ssize_t	key_len;

	i = 0;
	if (ft_isdigit(*key) || *key == '\0')
		return (e_error);
	key_len = ft_strlen(key);
	while (*key)
	{
		if ((i + 1) == key_len && *key == '+')
			return (e_append);
		if (!ft_isalnum(*key) && *key != '_')
			return (e_error);
		i++;
		key++;
	}
	return (e_add);
}

static t_export_info	*init_export_info(void)
{
	t_export_info	*e_info;

	e_info = malloc(sizeof(t_export_info));
	if (!e_info)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	e_info->key = NULL;
	e_info->value = NULL;
	e_info->skip_flag = 0;
	return (e_info);
}

int	export_cmd(t_info *info, t_export_info *e_info, char **cmds)
{
	int	exit_status;

	exit_status = define_key_value(*cmds, &(e_info->key), &(e_info->value),
			&(e_info->skip_flag));
	if (e_info->skip_flag == 0)
		e_info->key_type = judge_key(e_info->key);
	if (e_info->key_type == e_error && e_info->skip_flag != 1)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", *cmds);
		exit_status = EXIT_FAILURE;
	}
	if (e_info->key_type == e_append && e_info->skip_flag != 1)
		exit_status = append_env(info, e_info->key, e_info->value);
	if (e_info->key_type == e_add && e_info->skip_flag != 1)
		exit_status = add_env(info, e_info->key, e_info->value);
	return (exit_status);
}

int	ft_export(t_info *info, char **cmds)
{
	int				exit_status;
	t_export_info	*e_info;

	e_info = init_export_info();
	exit_status = EXIT_SUCCESS;
	if (!info || !cmds)
		return (EXIT_FAILURE);
	cmds++;
	//export NULL の挙動は未定義（shell変数を表示するが、実装しない！？なんのエラーを表示する？）
	if (*cmds == NULL)
		ft_dprintf(STDERR_FILENO, "minishell: export: invalid variable");
	while (*cmds != NULL)
	{
		exit_status = export_cmd(info, e_info, cmds);
		cmds++;
	}
	free(e_info);
	return (exit_status);
}
