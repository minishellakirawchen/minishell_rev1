/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_std.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:57:14 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:05:55 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STD_H
# define FT_STD_H

# include <stdbool.h>
# include <limits.h>

/* itoa atoi */
char		*ft_itoa(int n);
int			ft_atoi(const char *str, bool *is_success);
long long	ft_strtoll(char *num, bool *is_success);

#endif //FT_STD_H
