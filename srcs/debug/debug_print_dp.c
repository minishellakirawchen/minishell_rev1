/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print_dp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 20:34:56 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 20:47:47 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_dp_array(int **array, const char *wild, const char *target)
{
	size_t			i;
	size_t			j;
	const size_t	wild_len = ft_strlen_ns(wild);
	const size_t	str_len = ft_strlen_ns(target);

	i = 0;
	while (i < wild_len + 2)
	{
		j = 0;
		while (j < str_len + 2)
		{
			if ((i == 0 && j == 0) || (i == 1 && j == 0) || (i == 0 && j == 1))
				ft_printf("- ");
			else if (i == 0)
				ft_printf("%c ", target[j - 2]);
			else if (j == 0)
				ft_printf("%c ", wild[i - 2]);
			else
				ft_printf("%d ", array[i - 1][j - 1]);
			j++;
		}
		i++;
		ft_printf("\n");
	}
	ft_printf("\n");
}
