/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redirect_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 23:13:14 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 10:30:17 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static char	*get_filename_or_heredoc_eof(t_list_bdi **token_get_from, t_info *info);
static		t_redirect_list	*create_redirect_list(t_token_type type, char *str);

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
int	create_redirect_list_from_pipeline_tokens(t_command_list **cmd_list, t_info *info)
{
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;
	t_redirect_list	*redirect_list;
	t_token_type	type;
	char			*filename_or_heredoc_eof;
	t_list_bdi		*command_save;

	if (!cmd_list || !*cmd_list || !info)
		return (FAILURE);
	command_save = NULL;
	while ((*cmd_list)->pipeline_token_list)
	{
		popped_node = ft_lstpop_bdi(&(*cmd_list)->pipeline_token_list);
		token_elem = popped_node->content;
		if (is_tokentype_redirection(token_elem->type))
		{
			type = token_elem->type;
			ft_lstdelone_bdi(&popped_node, free_token_elem);
			filename_or_heredoc_eof = get_filename_or_heredoc_eof(&(*cmd_list)->pipeline_token_list, info);
			printf("filename or eof:%s\n", filename_or_heredoc_eof);
			redirect_list = create_redirect_list(type, filename_or_heredoc_eof);
			if (!filename_or_heredoc_eof || !redirect_list)
				return (FAILURE);
			(*cmd_list)->redirect_list = redirect_list;
			continue ;
		}
		ft_lstadd_back_bdi(&command_save, popped_node);
	}
	(*cmd_list)->pipeline_token_list = command_save;
	return (SUCCESS);
}

// redirect_list: do not split(re tokenize)
static t_redirect_list	*create_redirect_list(t_token_type type, char *str)
{
	t_redirect_list	*redirect_list;

	errno = 0;
	redirect_list = (t_redirect_list *)malloc(sizeof(t_redirect_list));
	if (!redirect_list)
		return (perror_ret_nullptr("malloc"));
	redirect_list->type = type;
	redirect_list->file = NULL;
	redirect_list->heredoc_eof = NULL;
	if (type == e_file)
		redirect_list->file = str;
	else
		redirect_list->heredoc_eof = str;
	return (redirect_list);
}

/* token node filename or heredoc eof is cleared in function */
static char	*get_filename_or_heredoc_eof(t_list_bdi **token_get_from, t_info *info)
{
	char			*str_concatted_token;
	t_list_bdi		*tmp_list;
	t_list_bdi		*popped_list;
	t_token_elem	*token_elem;

	if (!token_get_from || !*token_get_from || !info)
		return (NULL);
	tmp_list = NULL;
	while (*token_get_from)
	{
		popped_list = ft_lstpop_bdi(token_get_from);
		ft_lstadd_back_bdi(&tmp_list, popped_list);
		token_elem = popped_list->content;
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




































