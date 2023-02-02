/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 09:45:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/02 22:33:09 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

//bash-3.2 0 $ echo *		//in1 in2 ngfile out

//bash-3.2-2 0 $ echo *i	//*i				存在しない場合はnot expand
//bash-3.2-2 0 $ echo *i*	//in1 in2 ngfile	存在する場合は置換, is delim by token or space, spaceで良さそう？

//bash-3.2-2 0 $ export a2="a  b      c"
//bash-3.2-2 0 $ echo $a2	//a b c
//bash-3.2-2 0 $ cat $a2
//cat: a: No such file or directory
//cat: b: No such file or directory
//cat: c: No such file or directory

//bash-3.2-2 0 $ export b1="*"
//bash-3.2-2 0 $ export b2=i
//bash-3.2-2 0 $ echo $b2$b1
//in1 in2
//bash-3.2-2 0 $ echo $b1$b2
//*i
//bash-3.2-2 0 $

//expand var -> expand wild card
//unquotedされていてもOK? ->NG, "*" 'I' is *

//$b1$b2 が結合されたtokenに対して expand wildcardを展開、quotedには展開しない
//concatはどこで実施している...?
//get_expanded_strでconcat済みなので、expand var -> expand wildcard ->quote removalで良さそう

// tokenを検索, *が展開できれば置換する
// wildcard_str is one word
\
// sort as a to z
// echo  * :隠しファイルは表示しない
// echo .* :                する
// echo .*.:                しない
// -> .* or not
// matchしたものをリストに保持してsortする


/* return true if string consist of `.` and `*`,
 * `.` is the first half and `*` in the second half */
bool	check_is_hidden_include(const char *wildcard_str)
{
//	size_t	idx;

	if (!wildcard_str || wildcard_str[0] != '.')
		return (false);
//	idx = 0;
//	while (wildcard_str[idx] && wildcard_str[idx] == '.')
//		idx++;
//	if (wildcard_str[idx] && wildcard_str[idx] != '*')
//		return (false);
//	while (wildcard_str[idx] && wildcard_str[idx] == '*')
//		idx++;
//	if (wildcard_str[idx])
//		return (false);
	return (true);
}

/*
t_list_bdi	*get_strlist_matching_wildcard(const char *wildcard_str, DIR *dirp, int *valid_list)
{
	bool			is_hidden_include;
	struct dirent	*read_dir;
	t_list_bdi		*strlist;
	t_list_bdi		*newlist;
	char			*getstr;

	if (!wildcard_str || !dirp || !valid_list)
		return (NULL);
	is_hidden_include = check_is_hidden_include(wildcard_str);
	strlist = NULL;
	read_dir = readdir(dirp);
	while (read_dir)
	{
		if (read_dir->d_name[0] == '.' && !is_hidden_include)
		{
			read_dir = readdir(dirp);
			continue ;
		}
		if (is_matches_wildcard_and_target_str(wildcard_str, read_dir->d_name, valid_list))
		{
			printf("\n <wild card>\n");
			debug_print_wildcard_valid_list(valid_list, wildcard_str);
			getstr = ft_strdup(read_dir->d_name);
			printf(" @@ get_str:%s\n", getstr);
			newlist = ft_lstnew_bdi(getstr);
			if (!getstr || !newlist)
				return (perror_ret_nullptr("malloc"));// TODO: free
			ft_lstadd_back_bdi(&strlist, newlist);
		}
		read_dir = readdir(dirp);
	}
	return (strlist);
}
*/

void	swap(void **content_a, void **content_b)
{
	void *tmp;

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

void	print_list_str(void *content)
{
	printf("%s\n", (char *)content);
}

bool	is_expandable_wildcard_in_str(const char *word, bool is_quoted)
{
	if (cnt_chr_in_str('*', word) == 0 || is_quoted)
		return (false);
	return (true);
}

bool	is_not_expandable_command(t_list_bdi *first_token)
{
	t_token_elem	*token_elem;

	if (!first_token)
		return (false);
	token_elem = first_token->content;
	return (is_same_str("export", token_elem->word));
}

t_token_elem	*create_token_elem(char *word)
{
	t_token_elem	*new_token;

	new_token = (t_token_elem *)malloc(sizeof(t_token_elem));
	if (!new_token)
	{
		free_token_elem(new_token);
		return (perror_ret_nullptr("malloc"));
	}
	new_token->word = word;
	new_token->type = e_init;
	new_token->is_connect_to_next_word = false;
	new_token->is_quoted = false;
	new_token->quote_chr = '\0';
	new_token->subshell_depth = -1;
	new_token->wildcard_valid_flag = NULL;
	return (new_token);
}

t_list_bdi	*get_tokens_match_with_wildcard(const char *wildcard_str, t_list_bdi *dirlist, const int *valid_list)
{
	bool			is_hidden_include;
	t_list_bdi		*strlist;
	t_list_bdi		*newlist;
	t_token_elem	*token_elem;
	char			*name;
	t_list_bdi		*popped_node;

	if (!wildcard_str || !dirlist)
		return (NULL);
	is_hidden_include = check_is_hidden_include(wildcard_str);
	strlist = NULL;
	while (dirlist)
	{
		popped_node = ft_lstpop_bdi(&dirlist);
		name = popped_node->content;
		if (name[0] == '.' && !is_hidden_include)
		{
			ft_lstdelone_bdi(&popped_node, free);
			continue ;
		}
		if (is_matches_wildcard_and_target_str(wildcard_str, name, valid_list))
		{
			token_elem = create_token_elem(name);
			newlist = ft_lstnew_bdi(token_elem);
			if (!newlist || !token_elem)
				return (perror_ret_nullptr("malloc"));// TODO: free
			ft_lstadd_back_bdi(&strlist, newlist);
			ft_lstdelone_bdi(&popped_node, NULL);
			continue ;
		}
		ft_lstdelone_bdi(&popped_node, free);
	}
	debug_print_tokens(strlist, "strlist");
	return (strlist);
}

t_list_bdi	*get_read_dir_list(void)
{
	DIR 			*dir;
	char			*pwd_path;
	struct dirent	*dirent;
	t_list_bdi		*save_list;
	t_list_bdi		*new_node;
	char			*name;

	pwd_path = getcwd(NULL, 0);
	if (!pwd_path)
		return (perror_ret_nullptr("getcwd"));
	dir = opendir(pwd_path); // `.`でもOK？
	if (!dir)
		return (perror_ret_nullptr("opendir")); //TODO:free;
	save_list = NULL;
	dirent = readdir(dir);
	while (dirent)
	{
		name = ft_strdup(dirent->d_name);
		new_node = ft_lstnew_bdi(name);
		if (!name || !new_node)
			return (perror_ret_nullptr("malloc"));
		ft_lstadd_back_bdi(&save_list, new_node);
		dirent = readdir(dir);
	}
	free(pwd_path);
	closedir(dir);
	return (save_list);
}

int	get_wildcard_tokens(t_list_bdi **get_tokens_save_to, const char *wildcard_str, const int *valid_list)
{
	t_list_bdi	*dirlist;

	if (!get_tokens_save_to || !wildcard_str || !valid_list)
		return (FAILURE);

	dirlist = get_read_dir_list();
	if (!dirlist)
		return (FAILURE);
	*get_tokens_save_to = get_tokens_match_with_wildcard(wildcard_str, dirlist, valid_list);

	debug_print_tokens(*get_tokens_save_to, "before sorted");
	sort_ascending_strlist(&(*get_tokens_save_to));
	debug_print_tokens(*get_tokens_save_to, "after sorted");
	return (SUCCESS);
}



// * or "*"を判定し, *なら展開, "*"なら展開しない, ref token_elen->is_wildcard_expandable
// *, "*"が混在するケースは考慮していない。むずい。有効な*を何かに置換するなどの操作が必要か？
// もしくは結合前に、結合するグループを線型リストで保持しておき、
// 各tokenがquoted, unquotedのフラグを持っている状態で判別する？
int expanded_wildcard_to_token_list(t_list_bdi **token_list)
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
			if (get_wildcard_tokens(&wildcard_match_tokens, token_elem->word, token_elem->wildcard_valid_flag) == FAILURE)
				return (FAILURE);
			debug_print_tokens(wildcard_match_tokens, "wildcard tokens");
			if (ft_lstsize_bdi(wildcard_match_tokens) != 0)
			{
				ft_lstdelone_bdi(&popped_node, free_token_elem);
				popped_node = wildcard_match_tokens;
			}
		}
		ft_lstadd_back_bdi(&expanded_tokens, popped_node);
	}
	*token_list = expanded_tokens;
//	debug_print_tokens(*token_list, "wildcard expanded");
	return (SUCCESS);
}
