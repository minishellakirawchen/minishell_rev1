/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_in_filename.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 13:19:54 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 13:23:48 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static int	is_iotype_redirect(t_token_type type)
{
	return (type == e_redirect_in \
	|| type == e_redirect_out \
	|| type == e_redirect_append);
}

static int	get_expand_filename_to_redirect_info(\
t_redirect_info **redirect_info, char *ambiguous_word, t_info *info)
{
	char	**expand_or_re_tokenized_chars;

	expand_or_re_tokenized_chars = create_expanded_commands(\
	&(*redirect_info)->token_list, info, &ambiguous_word);
	if (!expand_or_re_tokenized_chars)
	{
		ambiguous_word = free_1d_alloc(ambiguous_word);
		return (FAILURE);
	}
	if (!ambiguous_word)
		return (FAILURE);
	if (get_2d_array_size(expand_or_re_tokenized_chars) != 1 \
	|| ft_strlen_ns(expand_or_re_tokenized_chars[0]) == 0)
	{
		(*redirect_info)->is_ambiguous = true;
		(*redirect_info)->filename = ft_strdup(ambiguous_word);
	}
	else
		(*redirect_info)->filename = ft_strdup(expand_or_re_tokenized_chars[0]);
	ambiguous_word = free_1d_alloc(ambiguous_word);
	free_2d_alloc((void **)expand_or_re_tokenized_chars);
	if (!(*redirect_info)->filename)
		return (perror_ret_int("malloc", FAILURE));
	return (SUCCESS);
}

int	expand_var_in_redirect_filename(t_command_info **cmd_list, t_info *info)
{
	t_list_bdi		*redirect_list;
	t_redirect_info	*redirect_info;
	char			*ambiguous_word;

	if (!cmd_list || !*cmd_list)
		return (FAILURE);
	redirect_list = (*cmd_list)->redirect_list;
	ambiguous_word = NULL;
	while (redirect_list)
	{
		redirect_info = redirect_list->content;
		if (is_iotype_redirect(redirect_info->io_type))
			if (get_expand_filename_to_redirect_info(\
			&redirect_info, ambiguous_word, info) == FAILURE)
				return (FAILURE);
		redirect_list = redirect_list->next;
	}
	return (SUCCESS);
}
