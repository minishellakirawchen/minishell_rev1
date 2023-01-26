/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:39:19 by takira            #+#    #+#             */
/*   Updated: 2022/11/07 14:00:00 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*ret_str;
	unsigned int	i;

	if (s == NULL || f == NULL)
		return (NULL);
	ret_str = (char *)ft_calloc(sizeof(char), ft_strlen(s) + 1);
	if (ret_str == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		ret_str[i] = f(i, s[i]);
		i++;
	}
	return (ret_str);
}
