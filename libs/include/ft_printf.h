/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:57:14 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 15:49:36 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <stdarg.h>
# include <errno.h>
# include <limits.h>

# include "ft_math.h"

# define SUCCESS 1
# define FAILURE 0

/* printf */
typedef struct s_pritnf_info
{
	size_t		fmt_idx;
	bool		flag_left;
	bool		flag_sign;
	bool		flag_space;
	bool		flag_prefix;
	bool		flag_zero_pad;
	bool		prec_dot;
	bool		prec_dot_only;
	bool		is_pointer;
	long long	width_siz;
	long long	perc_siz;
	int			num_base;
	int			num_capitals;
	size_t		num_padlen;
	size_t		num_preclen;
	char		*num_head_chr;
	int			fd;
}				t_printf_info;

int			ft_printf(const char *fmt, ...);
int			ft_dprintf(int fd, const char *fmt, ...);

int			printf_controller(\
t_printf_info *info, const char *fmt, va_list *ap);
int			get_width(char *fmt, t_printf_info *info, va_list *p);
int			get_prec(char *fmt, t_printf_info *info, va_list *p);
int			valid_info4fmt(char c, t_printf_info *info);
int			ft_isdigit_pf(int c);

ssize_t		ft_putchar_for_printf(char c, int fd);
ssize_t		ft_putstr_for_printf(char *s, int fd);
ssize_t		print_c(char chr, t_printf_info info);
ssize_t		print_s(char *str, t_printf_info info);
ssize_t		print_percent(t_printf_info info);
ssize_t		print_signed(int num, t_printf_info *info);
ssize_t		print_hex(unsigned long u, t_printf_info *info, bool isupper);
ssize_t		print_unsigned(unsigned long u, t_printf_info info);

size_t		ft_strlen_printf(const char *s);

char		*ft_strchr_printf(const char *s, int c);
void		get_flag(char *fmt, t_printf_info *info);

#endif //FT_PRINTF_H
