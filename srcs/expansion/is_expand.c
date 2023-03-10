/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 23:20:25 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 12:08:18 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/*
 judge $key in string
  str=$hoo$var$hoge
       ^^^ return true
*/
bool	is_name(const char *str)
{
	size_t	idx;

	if (!str || !str[0])
		return (false);
	if (str[0] != '_' && !ft_isalpha(str[0]))
		return (false);
	idx = 1;
	while (str[idx] && is_chr_in_str(str[idx], STR_EXPAND_EXIT_STATUS))
	{
		if (str[idx] != '_' && !ft_isalnum(str[idx]))
			return (false);
		idx++;
	}
	return (true);
}

// return true if $? or $name in str, do not depend on validate $name
bool	is_expandable_var(const char *str, char quote_chr)
{
	size_t	idx;

	if (!str || quote_chr == CHR_SINGLE_QUOTE)
		return (false);
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == CHR_DOLLAR && str[idx + 1])
		{
			if (str[idx + 1] == CHR_QUESTION)
				return (true);
			if (is_name(&str[idx + 1]))
				return (true);
		}
		idx++;
	}
	return (false);
}

bool	is_str_expandable(const char *str)
{
	if (!str)
		return (false);
	if (str[0] == CHR_DOLLAR && !ft_ispunct(str[1]))
		return (true);
	return (false);
}

bool	is_expandable_exit_status(const char *str)
{
	const size_t	len_flag = ft_strlen_ns(STR_EXPAND_EXIT_STATUS);

	if (ft_strncmp_ns(str, STR_EXPAND_EXIT_STATUS, len_flag) == 0)
		return (true);
	return (false);
}

bool	is_expandable_str_with_dollar(const char *str)
{
	if (!str || str[0] != CHR_DOLLAR)
		return (false);
	if (str[1] == CHR_QUESTION)
		return (true);
	if (str[1] == '_' || ft_isalnum(str[1]))
		return (true);
	return (false);
}
