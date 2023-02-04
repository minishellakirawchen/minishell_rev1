/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:22:41 by takira            #+#    #+#             */
/*   Updated: 2023/02/03 23:10:08 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* FREE OK */
#include "expansion.h"

/* prototype declaration */
static size_t	get_connected_token_size(t_list_bdi *list);
static char		**create_commands_from_token_list(t_list_bdi **token_list);

/* functions */

/*
 quote removal("')		["hello"] -> [hello]
  v
 expand var(except')	[$key] -> [var1  var2]
  v
 re tokenize(except"')	[var1  var2] -> [var1],[var2]
  v
 concat	tokens(by"')	[hello]=[world] -> [helloworld]
  v
 expand wildcard		[wi*] -> [wildcard],[window],...
  v
 create commands		[echo],[hello] -> {"echo", "hello", NULL}
*/

/* token list is freed in function */
/* *concat_str need free in call function even if error occurred */
/*
	debug_print_tokens(*token_list, "1) create_expanded_commands");
	debug_print_tokens(*token_list, "2) create_expanded_commands - remove quote");
	printf("3) create_expanded_commands - concat_str:%s", concat_str ? *concat_str : NULL);
	debug_print_tokens(*token_list, "4) create_expanded_commands - expand var");
	debug_print_tokens(*token_list, "5) create_expanded_commands - re tokenize");
	debug_print_tokens(*token_list, "6) create_expanded_commands - concat");
	debug_print_tokens(*token_list, "7) create_expanded_commands - expand wildcard");
*/
char	**create_expanded_commands(t_list_bdi **token_list, t_info *info, char **concat_str)
{
	char	**commands;

	if (!token_list || !info)
		return (NULL);
	if (remove_quote_in_tokens(&*token_list) == FAILURE)
		return (NULL);
	if (concat_str)
	{
		*concat_str = create_string_by_concat_tokens(*token_list);
		if (!*concat_str)
			return (NULL);
	}
	if (expand_var_in_token_word(&*token_list, info) == FAILURE)
		return (NULL);
	if (re_tokenize_tokens(&*token_list) == FAILURE)
		return (NULL);
	if (concat_connected_tokens_and_create_wildcard_valid_list(&*token_list) == FAILURE)
		return (NULL);
	if (expanded_wildcard_to_token_list(&*token_list) == FAILURE)
		return (NULL);
	commands = create_commands_from_token_list(&*token_list);
	if (!commands)
		return (NULL);
	return (commands);
}

/* token:[echo],[hello],[world] -> commands:{"echo", "hello", "world", NULL} */
static char	**create_commands_from_token_list(t_list_bdi **token_list)
{
	t_list_bdi		*popped_list;
	t_token_elem	*token_elem;
	char			**commands;
	size_t			size;
	size_t			idx;

	if (!token_list || !*token_list)
		return (NULL);

	size = get_connected_token_size(*token_list);
	commands = (char **)ft_calloc(sizeof(char *), size + 1);
	if (!commands)
		return (perror_ret_nullptr("malloc"));
	idx = 0;
	while (*token_list)
	{
		popped_list = ft_lstpop_bdi(token_list);
		token_elem = popped_list->content;
		commands[idx] = concat_dst_to_src(&commands[idx], token_elem->word);
		if (!commands[idx])
		{
			free_2d_alloc((void **)commands);
			return (perror_ret_nullptr("malloc"));
		}
		idx++;
		ft_lstdelone_bdi(&popped_list, free_token_elem);
	}
	return (commands);
}

/* input:["hello"world]
 * token:["hello"]=[world] <- size=2 */
static size_t	get_connected_token_size(t_list_bdi *list)
{
	size_t			size;
	t_token_elem	*token_elem;

	size = 0;
	while (list)
	{
		token_elem = list->content;
		while (list && token_elem->is_connect_to_next_word)
		{
			list = list->next;
			if (list)
				token_elem = list->content;
		}
		size++;
		list = list->next;
	}
	return (size);
}
