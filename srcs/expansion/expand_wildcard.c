/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 09:45:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/01 17:18:43 by takira           ###   ########.fr       */
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



































