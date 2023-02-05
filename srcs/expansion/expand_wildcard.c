/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 09:45:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 12:31:02 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/* return true if string consist of `.` and `*`,
 * `.` is the first half and `*` in the second half */
bool	is_wildcard_includes_hidden(const char *wildcard_str)
{
	if (wildcard_str && wildcard_str[0] == '.')
		return (true);
	return (false);
}

void	swap(void **content_a, void **content_b)
{
	void	*tmp;

	if (!content_a || !content_b)
		return ;
	tmp = *content_a;
	*content_a = *content_b;
	*content_b = tmp;
}

void	sort_ascending_strlist(t_list_bdi **list_head)
{
	t_list_bdi		*node_a;
	t_list_bdi		*node_b;
	t_token_elem	*token_a;
	t_token_elem	*token_b;

	if (!list_head || !*list_head)
		return ;
	node_a = *list_head;
	while (node_a)
	{
		node_b = node_a->next;
		while (node_b)
		{
			token_a = node_a->content;
			token_b = node_b->content;
			if (ft_strcmp_ns(token_a->word, token_b->word) > 0)
				swap(&node_a->content, &node_b->content);
			node_b = node_b->next;
		}
		node_a = node_a->next;
	}
}

bool	is_not_expandable_command(t_list_bdi *first_token)
{
	t_token_elem	*token_elem;

	if (!first_token)
		return (false);
	token_elem = first_token->content;
	return (is_same_str("export", token_elem->word));
}

int	get_tokens_match_with_wildcard(t_list_bdi **save, \
const char *wildcard_str, t_list_bdi *dirlist, const int *valid_list)
{
	t_list_bdi		*newlist;
	t_token_elem	*token_elem;
	char			*name;
	t_list_bdi		*popped_node;

	if (!wildcard_str || !dirlist || !save)
		return (FAILURE);
	while (dirlist)
	{
		popped_node = ft_lstpop_bdi(&dirlist);
		name = popped_node->content;
		if (!is_wildcard_includes_hidden(wildcard_str) && name && name[0] == '.')
		{
			ft_lstdelone_bdi(&popped_node, free);
			continue ;
		}
		if (is_matches_wildcard_and_target_str(wildcard_str, name, valid_list))
		{
			token_elem = create_token_elem(name, false, false, '\0');
			if (!token_elem)
			{
				ft_lstdelone_bdi(&popped_node, NULL);
				return (FAILURE);
			}
			newlist = ft_lstnew_bdi(token_elem);
			if (!newlist || !token_elem)
			{
				ft_lstdelone_bdi(&popped_node, NULL);
				return (perror_ret_int("malloc", FAILURE));
			}
			ft_lstadd_back_bdi(&(*save), newlist);
			ft_lstdelone_bdi(&popped_node, NULL);
		}
		else
			ft_lstdelone_bdi(&popped_node, free);
	}
	return (SUCCESS);
}

static DIR	*get_dir(void)
{
	DIR				*dir;
	char			*pwd_path;

	errno = 0;
	pwd_path = getcwd(NULL, 0);
	if (!pwd_path)
		return (perror_ret_nullptr("getcwd"));
	dir = opendir(pwd_path);
	free(pwd_path);
	if (!dir)
		return (perror_ret_nullptr("opendir"));
	return (dir);
}

static t_list_bdi	*get_new_node(struct dirent *dirent)
{
	char		*name;
	t_list_bdi	*new_node;

	errno = 0;
	if (!dirent)
		return (NULL);
	name = ft_strdup(dirent->d_name);
	if (!name)
		return (perror_ret_nullptr("malloc"));
	new_node = ft_lstnew_bdi(name);
	if (!new_node)
	{
		free(name);
		return (perror_ret_nullptr("malloc"));
	}
	return (new_node);
}

t_list_bdi	*get_read_dir_list(void)
{
	DIR				*dir;
	struct dirent	*dirent;
	t_list_bdi		*save_list;
	t_list_bdi		*new_node;

	dir = get_dir();
	if (!dir)
		return (NULL);
	save_list = NULL;
	dirent = readdir(dir);
	while (dirent)
	{
		new_node = get_new_node(dirent);
		if (!new_node)
		{
			ft_lstclear_bdi(&save_list, free);
			closedir(dir);
			return (perror_ret_nullptr("malloc"));
		}
		ft_lstadd_back_bdi(&save_list, new_node);
		dirent = readdir(dir);
	}
	closedir(dir);
	return (save_list);
}

int	get_wildcard_tokens(t_list_bdi **get_tokens_save_to, \
const char *wildcard_str, const int *valid_list)
{
	t_list_bdi	*dirlist;

	if (!get_tokens_save_to || !wildcard_str || !valid_list)
		return (FAILURE);
	dirlist = get_read_dir_list();
	if (!dirlist)
		return (FAILURE);
	*get_tokens_save_to = NULL;
	if (get_tokens_match_with_wildcard(\
	get_tokens_save_to, wildcard_str, dirlist, valid_list) == FAILURE)
	{
		ft_lstclear_bdi(&dirlist, free);
		return (FAILURE);
	}
	sort_ascending_strlist(&(*get_tokens_save_to));
	return (SUCCESS);
}

// * or "*"を判定し, *なら展開, "*"なら展開しない, ref token_elen->is_wildcard_expandable
// *, "*"が混在するケースは考慮していない。むずい。有効な*を何かに置換するなどの操作が必要か？
// もしくは結合前に、結合するグループを線型リストで保持しておき、
// 各tokenがquoted, unquotedのフラグを持っている状態で判別する？
int	expanded_wildcard_to_token_list(t_list_bdi **token_list)
{
	t_list_bdi		*expanded_tokens;
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;
	t_list_bdi		*wildcard_match_tokens;

	if (!token_list)
		return (FAILURE);
	if (is_not_expandable_command(*token_list))
		return (SUCCESS);
	expanded_tokens = NULL;
	while (*token_list)
	{
		popped_node = ft_lstpop_bdi(&(*token_list));
		token_elem = popped_node->content;
		if (check_valid_wildcard_in_word(token_elem))
		{
			if (get_wildcard_tokens(&wildcard_match_tokens, \
			token_elem->word, token_elem->wildcard_valid_flag) == FAILURE)
				return (FAILURE);
			if (ft_lstsize_bdi(wildcard_match_tokens) != 0)
			{
				ft_lstdelone_bdi(&popped_node, free_token_elem);
				popped_node = wildcard_match_tokens;
			}
		}
		ft_lstadd_back_bdi(&expanded_tokens, popped_node);
	}
	*token_list = expanded_tokens;
	return (SUCCESS);
}
