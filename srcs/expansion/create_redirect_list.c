/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redirect_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 23:13:14 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 16:11:43 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static t_redirect_info	*create_redirect_list(t_token_type io_type, t_token_type str_type, char *str, bool is_expansion);
static char	*get_filename_or_heredoc_eof(t_list_bdi **token_get_from, t_info *info, t_token_type *type, bool *is_expand);

// pipeline_token_list		->	expand				->	split					->	char **commands
// ------------------------------------------------------------------------------------------------------------------------
// [echo]->[hello]->[world]															{"echo", "hello", "world", NULL}
// [$key]					->	[echo hello world]	->	[echo]->[hello]->[world]	->	{"echo", "hello", "world", NULL}
// ["hello"]=[world]
//
//
//
//bash-3.2$ export e1="echo hello"world'huga'
//bash-3.2$ echo $e1  # echo helloworldhuga
//;
//bash-3.2$ export e2="test   a  $e1'hogehoge'"
//bash-3.2$ echo $e2  # test a echo helloworldhuga'hogehoge'
//
//bash-3.2$ export x1=ech"o hello"
//bash-3.2$ $x1       # hello


/* redirect token node clear in function */
int	create_redirect_list_from_pipeline_tokens(t_command_info **cmd_list, t_info *info)
{
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;
	t_redirect_info	*redirect_info;
	t_list_bdi		*new_redirect_list;
	t_token_type	io_type;
	t_token_type	str_type;
	char			*filename_or_heredoc_eof;
	t_list_bdi		*command_save;
	bool			is_expand;

	if (!cmd_list || !*cmd_list || !info)
		return (FAILURE);
	command_save = NULL;
	while ((*cmd_list)->pipeline_token_list)
	{
		popped_node = ft_lstpop_bdi(&(*cmd_list)->pipeline_token_list);
		token_elem = popped_node->content;
		if (is_tokentype_redirection(token_elem->type))
		{
//			type = token_elem->type;
			io_type = token_elem->type;
//			printf("\n%s,type:%s\n", token_elem->word, token_elem->type == e_file ? "file" : "heredoc");
			ft_lstdelone_bdi(&popped_node, free_token_elem);

			filename_or_heredoc_eof = get_filename_or_heredoc_eof(&(*cmd_list)->pipeline_token_list, info, &str_type, &is_expand);
//			printf("filename or eof:%s\n", filename_or_heredoc_eof);

			redirect_info = create_redirect_list(io_type, str_type, filename_or_heredoc_eof, is_expand);
			new_redirect_list = ft_lstnew_bdi(redirect_info);
			if (!filename_or_heredoc_eof || !redirect_info || !new_redirect_list)
				return (FAILURE);
			ft_lstadd_back_bdi(&(*cmd_list)->redirect_list, new_redirect_list);
			continue ;
		}
		ft_lstadd_back_bdi(&command_save, popped_node);
	}
	(*cmd_list)->pipeline_token_list = command_save;
	return (SUCCESS);
}

// redirect_list: do not split(re tokenize)
static t_redirect_info	*create_redirect_list(t_token_type io_type, t_token_type str_type, char *str, bool is_expansion)
{
	t_redirect_info	*redirect_info;

	errno = 0;
	redirect_info = (t_redirect_info *)malloc(sizeof(t_redirect_info));
	if (!redirect_info)
		return (perror_ret_nullptr("malloc"));
	redirect_info->io_type = io_type;
	redirect_info->file = NULL;
	redirect_info->heredoc_eof = NULL;
	redirect_info->is_expansion = is_expansion;
	if (str_type == e_file)
		redirect_info->file = str;
	else
		redirect_info->heredoc_eof = str;
	return (redirect_info);
}

/* token node filename or heredoc eof is cleared in function */
static char	*get_filename_or_heredoc_eof(t_list_bdi **token_get_from, t_info *info, t_token_type *type, bool *is_expand)
{
	char			*str_concatted_token;
	t_list_bdi		*tmp_list;
	t_list_bdi		*popped_list;
	t_token_elem	*token_elem;

	*type = e_init;
	*is_expand = false;
	if (!token_get_from || !*token_get_from || !info)
		return (NULL);
	tmp_list = NULL;
	while (*token_get_from)
	{
		popped_list = ft_lstpop_bdi(token_get_from);
		ft_lstadd_back_bdi(&tmp_list, popped_list);
		token_elem = popped_list->content;
		if (*type == e_init)
		{
			*type = token_elem->type;
			*is_expand = token_elem->is_quoted;
		}
		if (!token_elem->is_connect_to_next_word)
			break ;
	}
	if (expand_var_in_tokens(&tmp_list, info) == FAILURE)
		return (NULL);
	str_concatted_token = concat_tokens(tmp_list);
	if (!str_concatted_token)
		return (NULL);
	ft_lstclear_bdi(&tmp_list, free_token_elem);
	return (str_concatted_token);
}




































