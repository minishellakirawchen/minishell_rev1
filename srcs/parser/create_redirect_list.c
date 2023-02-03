/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redirect_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 23:13:14 by takira            #+#    #+#             */
/*   Updated: 2023/02/02 23:08:50 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/* prototype declaration */
static int				create_redirect_list_from_pipeline_tokens(t_command_info **cmd_list);
static t_list_bdi		*get_filename_or_eof_tokenlist(t_list_bdi **token_get_from);
static int				create_heredoc_eof_from_tokens(t_command_info **cmd_list, t_info *info);

/* functions */
int	create_redirect_list(t_exec_list **exexlist_head, t_info *info)
{
	t_list_bdi		*command_list_node;
	t_command_info	*command_list;
	t_exec_list		*exec_list_node;


	if (!exexlist_head || !*exexlist_head)
		return (FAILURE);
	exec_list_node = *exexlist_head;
	while (exec_list_node)
	{
		command_list_node = exec_list_node->pipeline_commands;
		while (command_list_node)
		{
			command_list = command_list_node->content;
			if (create_redirect_list_from_pipeline_tokens(&command_list) == FAILURE)
				return (FAILURE);
			if (create_heredoc_eof_from_tokens(&command_list, info) == FAILURE)
				return (FAILURE);
//			printf("createe_redirect_list\n");
//			debug_print_command_info(command_list);
			command_list_node = command_list_node->next;
		}
		exec_list_node = exec_list_node->next;
	}
	return (SUCCESS);
}

// pipeline_token_list		->	expand				->	split					->	char **commands
// ------------------------------------------------------------------------------------------------------------------------
// [echo]->[hello]->[world]															{"echo", "hello", "world", NULL}
// [$key]					->	[echo hello world]	->	[echo]->[hello]->[world]	->	{"echo", "hello", "world", NULL}
// ["hello"]=[world]
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

// tokenをredirect_info->token_listに入れていく
static int	create_redirect_list_from_pipeline_tokens(t_command_info **cmd_list)
{
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;
	t_redirect_info	*redirect_info;
	t_list_bdi		*new_redirect_list;
	t_list_bdi		*command_save;

	if (!cmd_list || !*cmd_list)
		return (FAILURE);
	command_save = NULL;
	while ((*cmd_list)->pipeline_token_list)
	{
		popped_node = ft_lstpop_bdi(&(*cmd_list)->pipeline_token_list);
		token_elem = popped_node->content;

		/* type=redirect_in/out/append/heredoc, create redirect_info and move tokens */
		if (is_tokentype_redirection(token_elem->type))
		{
			redirect_info = create_redirect_info(token_elem->type, &(*cmd_list)->pipeline_token_list);
			new_redirect_list = ft_lstnew_bdi(redirect_info);
			if (!redirect_info || !new_redirect_list)
				return (FAILURE); //TODO:free
			ft_lstdelone_bdi(&popped_node, free_token_elem);
			ft_lstadd_back_bdi(&(*cmd_list)->redirect_list, new_redirect_list);
			continue ;
		}
		/* if token type is not redirect */
		ft_lstadd_back_bdi(&command_save, popped_node);
	}
	(*cmd_list)->pipeline_token_list = command_save;
	return (SUCCESS);
}

/* token node filename or heredoc eof is cleared in function */
static t_list_bdi	*get_filename_or_eof_tokenlist(t_list_bdi **token_get_from)
{
	t_list_bdi		*token_list;
	t_list_bdi		*popped_token_node;
	t_token_elem	*token_elem;

	if (!token_get_from || !*token_get_from)
		return (NULL);
	token_list = NULL;
	while (*token_get_from)
	{
		popped_token_node = ft_lstpop_bdi(token_get_from);
		token_elem = popped_token_node->content;
		ft_lstadd_back_bdi(&token_list, popped_token_node);
		if (!token_elem->is_connect_to_next_word)
			break ;
	}
	return (token_list);
}

t_redirect_info	*create_redirect_info(t_token_type io_type, t_list_bdi **token_list)
{
	t_redirect_info	*redirect_info;

	if (!token_list || !*token_list)
		return (NULL);
	errno = 0;
	redirect_info = (t_redirect_info *)malloc(sizeof(t_redirect_info));
	if (!redirect_info)
		return (perror_ret_nullptr("malloc"));
	redirect_info->io_type = io_type;
	redirect_info->filename = NULL;
	redirect_info->heredoc_eof = NULL;
	redirect_info->is_expansion = true;
	redirect_info->is_ambiguous = false;
	redirect_info->token_list = get_filename_or_eof_tokenlist(token_list);
	return (redirect_info);
}

static int	create_heredoc_eof_from_tokens(t_command_info **cmd_list, t_info *info)
{
	t_list_bdi		*node;
	t_redirect_info	*redirect_info;
	bool			is_quoted;

	if (!cmd_list || !*cmd_list)
		return (FAILURE);
	node = (*cmd_list)->redirect_list;
	while (node)
	{
		redirect_info = node->content;
		if (redirect_info->io_type == e_heredoc)
		{
			redirect_info->heredoc_eof = get_filename_or_heredoc_eof(&redirect_info->token_list, &is_quoted, false, info);
			if (!redirect_info->heredoc_eof)
				return (FAILURE);
			if (is_quoted)
				redirect_info->is_expansion = false;
		}
		node = node->next;
	}
	return (SUCCESS);
}

/* token node filename or heredoc eof is cleared in function */
char	*get_filename_or_heredoc_eof(t_list_bdi **token_get_from, bool *is_quoted, bool is_expand, t_info *info)
{
	char			*str_concatted_token;
	t_list_bdi		*token_list;
	t_list_bdi		*popped_token_node;
	t_token_elem	*token_elem;

	if (!token_get_from || !*token_get_from)
		return (NULL);
	*is_quoted = false;
	token_list = NULL;
	while (*token_get_from)
	{
		popped_token_node = ft_lstpop_bdi(token_get_from);
		token_elem = popped_token_node->content;
		if (is_expand && is_expandable_var_in_str(token_elem->word, token_elem->quote_chr))
			if (expand_var_in_str(&token_elem->word, info) == FAILURE)
				return (FAILURE);
		ft_lstadd_back_bdi(&token_list, popped_token_node);
		*is_quoted |= token_elem->is_quoted;
		if (!token_elem->is_connect_to_next_word)
			break ;
	}
	if (remove_quote_in_tokens(&token_list) == FAILURE)
		return (NULL);
	str_concatted_token = create_string_by_concat_tokens(token_list);
	if (!str_concatted_token)
		return (NULL);
	ft_lstclear_bdi(&token_list, free_token_elem);
	return (str_concatted_token);
}
