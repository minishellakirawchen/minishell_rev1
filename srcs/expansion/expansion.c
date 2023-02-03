/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:03:13 by takira            #+#    #+#             */
/*   Updated: 2023/02/02 23:20:18 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static int	expand_var_in_redirect_filename(t_command_info **cmd_list, t_info *info);

int	expand_var_and_create_commands_from_tokens(t_exec_list **pipeline, t_info *info)
{
	t_list_bdi		*cmd_list_node;
	t_command_info	*cmd_info;

	if (!pipeline || !info)
		return (FAILURE);
	cmd_list_node = (*pipeline)->pipeline_commands;
	while (cmd_list_node)
	{
		cmd_info = cmd_list_node->content;
		cmd_info->commands = create_expanded_commands(&cmd_info->pipeline_token_list, info, NULL);
		if (!cmd_info->commands)
			return (FAILURE);
		if (expand_var_in_redirect_filename(&cmd_info, info) == FAILURE)
			return (FAILURE);
		cmd_list_node = cmd_list_node->next;
	}
	return (SUCCESS);
}

//bash3.2 0 $ echo "$a1"	//hoge    hoge
//bash3.2 0 $ echo $a1		//hoge hoge

//bash3.2 0 $ echo hello>$a1
//bash: $a1: ambiguous redirect

//bash3.2 1 $ echo hello>"$a1"
//bash3.2 0 $ ls
//hoge    hoge

//bash3.2 0 $ echo hello >'$a1'
//bash3.2 0 $ ls
//$a1

// command_list->redirect_list = heredoc->io->io->heredoc->...
// if type=io, expand and create filename from redirect_list->content=redirect_info->token_list

// echo hello>"$a1"	->$a1 = "hoge    huga"
// echo hello>"a1	->$a1 = "hoge", "huga" -> ambiguous error

size_t	get_2darray_size(char **array)
{
	size_t	idx;

	idx = 0;
	while (array && array[idx])
		idx++;
	return (idx);
}

void	set_info_params_for_ambiguous_error(t_redirect_info	**redirect_info)
{
	t_token_elem	*token_elem;

	if (!redirect_info || !*redirect_info || !(*redirect_info)->token_list)
		return ;
	token_elem = (*redirect_info)->token_list->content;
	(*redirect_info)->is_ambiguous = true;
	(*redirect_info)->filename = token_elem->word;
}

int	is_iotype_redirect(t_token_type type)
{
	return (type == e_redirect_in || type == e_redirect_out || type == e_redirect_append);
}


// どの状態のものを警告に出せば良いんだ？？わからなくなってきた
//bash3.2 1 $ export x="hello world"
//bash3.2 0 $ echo hello>"$x"	//file "hello world"
//bash3.2 0 $ echo hello>$x		//bash: $x: ambiguous redirect
//bash3.2 0 $ echo hello>"**"	//file "**"
//bash3.2 0 $ echo hello>'**'	// file "**"
//bash3.2 0 $ echo hello>**		//bash: **: ambiguous redirect

//bash3.2 0 $ echo hello >$xhoge	//bash: $xhoge: ambiguous redirect
//bash3.2 1 $ echo hello >$x"hoge"	//bash: $x"hoge": ambiguous redirect
//bash3.2 1 $ echo $xhoge			//
//bash3.2 0 $ echo >""				//bash: : No such file or directory
//>"" と >$nothing は"" vs NULL？

//declare -x a=""
//declare -x b
//bash3.2 0 $ echo hello >$a	//bash: $a: ambiguous redirect
//bash3.2 1 $ echo hello >$b	//bash: $b: ambiguous redirect
//うん？
// expandによる""と、文字列""を識別する必要がありそう

// after concat, before expand wildcardの状態が欲しい
// concatされてone wordのはず


// redirectはconcat->expandか?
// commandはexpand->concat
//bash-3.2-2 0 $ export b1="$"
//bash-3.2-2 0 $ export b2="b3"
//bash-3.2-2 0 $ export b3=b3dayo
//bash-3.2-2 0 $ echo $b1$b2
//$b3
//
//redirectも同様にconcat->expandだった
//bash-3.2-2 0 $ echo hello>$b1$b2
//bash-3.2-2 0 $ ls
//$b3             dir1            hello  world    out             outout
//bash-3.2-2 0 $

//echo hello >"hello"'world'	= helloworld
//echo hello >"hello"'  world'	= hello   world

//bash-3.2-2 0 $ echo $a1	//hello world
//bash-3.2-2 0 $ echo "$a1"	//hello  world
//bash-3.2-2 0 $ echo "$a2"	//hello
//bash-3.2-2 0 $ echo "$a3"	//world

//bash-3.2-2 0 $ echo hello >$a1	//bash: $a1: ambiguous redirect
//bash-3.2-2 1 $ echo hello >$a2$a3	//helloworld
//bash-3.2-2 0 $ echo $a2$a3
//helloworld
//bash-3.2-2 0 $ echo "$a2$a3"
//helloworld
//bash-3.2-2 0 $ echo hello >$a1$a2$a3		//bash: $a1$a2$a3: ambiguous redirect
//bash-3.2-2 1 $ echo hello >"$a1$a2$a3"	//hello  worldhelloworld

//bash-3.2-2 0 $ export c1="*"
//bash-3.2-2 0 $ echo hello>$c1		//bash: $c1: ambiguous redirect
//bash-3.2-2 1 $ echo hello>"$c1"	//file:*

//concatteed filename
//

//filenameはone wordで受け取る。""''も含む
//""''はone wordとしてexpansion and quote removal, $hogeはspace split
// one wordならfilename, multi wordならambiguous


//unquoted && connectした文字列を保持


static int	expand_var_in_redirect_filename(t_command_info **cmd_list, t_info *info)
{
	char			**expand_or_re_tokenized_chars;
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
		{
			expand_or_re_tokenized_chars = create_expanded_commands(&redirect_info->token_list, info, &ambiguous_word);
			if (!expand_or_re_tokenized_chars)
				return (FAILURE);
			if (!ambiguous_word)
				return (FAILURE);
			if (get_2darray_size(expand_or_re_tokenized_chars) != 1 || ft_strlen_ns(expand_or_re_tokenized_chars[0]) == 0)
			{
				redirect_info->is_ambiguous = true;
				redirect_info->filename = ft_strdup(ambiguous_word);
				ambiguous_word = free_1d_alloc(ambiguous_word);
			}
			else
				redirect_info->filename = ft_strdup(expand_or_re_tokenized_chars[0]);
			if (!redirect_info->filename)
				return (perror_ret_int("malloc", FAILURE));//TODO:free
			free_2d_alloc((void **)expand_or_re_tokenized_chars);
		}
		redirect_list = redirect_list->next;
	}
//	debug_print_redirect_info((*cmd_list)->redirect_list, "expand_var_in_redirect");
	return (SUCCESS);
}
