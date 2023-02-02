/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 09:45:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/02 13:33:34 by takira           ###   ########.fr       */
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
	size_t	idx;

	if (!wildcard_str || wildcard_str[0] != '.')
		return (false);
	idx = 0;
	while (wildcard_str[idx] && wildcard_str[idx] == '.')
		idx++;
	if (wildcard_str[idx] && wildcard_str[idx] != '*')
		return (false);
	while (wildcard_str[idx] && wildcard_str[idx] == '*')
		idx++;
	if (wildcard_str[idx])
		return (false);
	return (true);
}

t_list_bdi	*get_strlist_matching_wildcard(const char *wildcard_str, DIR *dirp)
{
	bool			is_hidden_include;
	struct dirent	*read_dir;
	t_list_bdi		*strlist;
	t_list_bdi		*newlist;
	char			*getstr;

	if (!wildcard_str || !dirp)
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
		if (is_matches_wildcard_and_target_str(wildcard_str, read_dir->d_name))
		{
			getstr = ft_strdup(read_dir->d_name);
			printf("get_str:%s\n", getstr);
			newlist = ft_lstnew_bdi(getstr);
			if (!getstr || !newlist)
				return (perror_ret_nullptr("malloc"));// TODO: free
			ft_lstadd_back_bdi(&strlist, newlist);
		}
		read_dir = readdir(dirp);
	}
	return (strlist);
}

void	sort_ascending_strlist(t_list_bdi **list_head)
{
	t_list_bdi	*node_a;
	t_list_bdi	*node_b;
	char		*str_a;
	char		*str_b;
	char 		*tmp_str;

	if (!list_head || !*list_head)
		return ;
	node_a = *list_head;
	while (node_a)
	{
		node_b = node_a->next;
		while (node_b)
		{
			str_a = node_a->content;
			str_b = node_b->content;
			if (ft_strcmp_ns(str_a, str_b) > 0)
			{
				tmp_str = node_a->content;
				node_a->content = node_b->content;
				node_b->content = tmp_str;
			}
			node_b = node_b->next;
		}
		node_a = node_a->next;
	}
}

char	*get_list_concat_str(t_list_bdi *list, char *separated_str)
{
	char	*concat_str;

	if (!list)
		return (NULL);
	concat_str = ft_strdup("");
	if (!concat_str)
		return (perror_ret_nullptr("malloc"));

	while (list)
	{
		concat_str = concat_dst_to_src(&concat_str, (char *)list->content);
		if (!concat_str)
			return (perror_ret_nullptr("malloc")); //TODO:free
		list = list->next;
		if (list && separated_str)
		{
			concat_str = concat_dst_to_src(&concat_str, separated_str);
			if (!concat_str)
				return (perror_ret_nullptr("malloc")); //TODO:free
		}
	}

	return (concat_str);
}

void	print_list_str(void *content)
{
	printf("%s\n", (char *)content);
}

// 一致するstringがない場合、stringを返す
char	*get_expand_wildcard(char *wildcard_str)
{
	DIR 			*dirp;
	t_list_bdi		*strlist;
	char			*pwd_path;
	char			*expanded_str;

	if (!wildcard_str)
		return (NULL);
	pwd_path = getcwd(NULL, 0);
	if (!pwd_path)
		return (perror_ret_nullptr("getcwd"));
	dirp = opendir(pwd_path); // `.`でもOK？
	if (!dirp)
		return (perror_ret_nullptr("opendir")); //TODO:free;
	strlist = get_strlist_matching_wildcard(wildcard_str, dirp);
	closedir(dirp);
	free(pwd_path);

//	printf("\n## before sort\n");
//	ft_lstiter_bdi(strlist, print_list_str);

	if (ft_lstsize_bdi(strlist) == 0)
		return (wildcard_str);

	sort_ascending_strlist(&strlist);

//	printf("\n## after sort\n");
//	ft_lstiter_bdi(strlist, print_list_str);

	expanded_str = get_list_concat_str(strlist, " ");
	if (!expanded_str)
		return (perror_ret_nullptr("malloc")); //TODO:free
	ft_lstclear_bdi(&strlist, free);
	free_1d_alloc(wildcard_str);
	return (expanded_str);
}

bool	is_expandable_wildcard_in_str(const char *word, bool is_quoted)
{
	if (cnt_chr_in_str('*', word) == 0 || is_quoted)
		return (false);
	return (true);
}

bool	is_command_export(t_list_bdi *first_token)
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
	new_token->is_wildcard_quoted = false;
	return (new_token);
}

t_list_bdi	*get_tokens_match_with_wildcard(const char *wildcard_str, DIR *dirp)
{
	bool			is_hidden_include;
	struct dirent	*read_dir;
	t_list_bdi		*strlist;
	t_list_bdi		*newlist;
	char			*getstr;
	t_token_elem	*token_elem;

	if (!wildcard_str || !dirp)
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
		if (is_matches_wildcard_and_target_str(wildcard_str, read_dir->d_name))
		{
			getstr = ft_strdup(read_dir->d_name);
			token_elem = create_token_elem(getstr);
			newlist = ft_lstnew_bdi(token_elem);
			if (!getstr || !newlist || !token_elem)
				return (perror_ret_nullptr("malloc"));// TODO: free
			ft_lstadd_back_bdi(&strlist, newlist);
		}
		read_dir = readdir(dirp);
	}
	return (strlist);
}

int	get_wildcard_tokens(t_list_bdi **get_tokens_save_to, const char *wildcard_str)
{
	DIR 			*dirp;
	char			*pwd_path;

	if (!get_tokens_save_to || !wildcard_str)
		return (FAILURE);

	pwd_path = getcwd(NULL, 0);
	if (!pwd_path)
		return (perror_ret_int("getcwd", FAILURE));
	dirp = opendir(pwd_path); // `.`でもOK？
	if (!dirp)
		return (perror_ret_int("opendir", FAILURE)); //TODO:free;
	*get_tokens_save_to = get_tokens_match_with_wildcard(wildcard_str, dirp);

	closedir(dirp);
	free(pwd_path);

//	debug_print_tokens(*get_tokens_save_to, "before sorted");
	sort_ascending_strlist(get_tokens_save_to);
//	debug_print_tokens(*get_tokens_save_to, "after sorted");
	return (SUCCESS);
}

// * or "*"を判定し, *なら展開, "*"なら展開しない, ref token_elen->is_wildcard_quoted
// *, "*"が混在するケースは考慮していない。むずい。有効な*を何かに置換するなどの操作が必要か？
// もしくは結合前に、結合するグループを線型リストで保持しておき、
// 各tokenがquoted, unquotedのフラグを持っている状態で判別する？
int expanded_wildcard_to_token_list(t_list_bdi **token_list)
{
	t_list_bdi		*expanded_tokens;
	t_list_bdi		*popped_node;
	t_token_elem	*token_elem;
	t_list_bdi		*wildcard_tokens;

	if (!token_list)
		return (FAILURE);
	if (is_command_export(*token_list))
		return (SUCCESS);
	expanded_tokens = NULL;
	while (*token_list)
	{
		popped_node = ft_lstpop_bdi(&(*token_list));
		token_elem = popped_node->content;
		if (is_expandable_wildcard_in_str(token_elem->word, !token_elem->is_wildcard_quoted))
		{
			wildcard_tokens = NULL;
			if (get_wildcard_tokens(&wildcard_tokens, token_elem->word) == FAILURE)
				return (FAILURE);
			debug_print_tokens(wildcard_tokens, "wildcard tokens");
			if (ft_lstsize_bdi(wildcard_tokens) != 0)
			{
				ft_lstclear_bdi(&popped_node, free_token_elem);
				popped_node = wildcard_tokens;
			}
		}
		ft_lstadd_back_bdi(&expanded_tokens, popped_node);
	}
	*token_list = expanded_tokens;
//	debug_print_tokens(*token_list, "wildcard expanded");
	return (SUCCESS);
}





















