/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 10:46:22 by takira            #+#    #+#             */
/*   Updated: 2023/01/29 01:24:03 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <errno.h>
# include <limits.h>
# include <fcntl.h>
# include <stdarg.h>

# define BUFFER_SIZE 256

# define PASS 1
# define FAIL 0

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

/* is */
int			ft_isupper(int c);
int			ft_islower(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
int			ft_isspace(char c);

bool		is_chr_in_str(char chr, const char *str);
bool		is_str1chrs_in_str2(const char *str1, const char *str2);

/* to */
int			ft_toupper(int c);
int			ft_tolower(int c);

/* str */
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strncmp_ns(const char *s1, const char *s2, size_t n);
bool		is_same_str(const char *str1, const char *str2);

size_t		ft_strlen(const char *s);
size_t		ft_strlen_ns(const char *s);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);

char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strdup(const char *s1);
char		*ft_strdup_ns(const char *s1);
char		*ft_strtrim(char const *s1, char const *set);

char		**ft_split(char const *s, char c);
char		**ft_split_set(char const *s, char delim, char set);

char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));

/* itoa atoi */
char		*ft_itoa(int n);
int			ft_atoi(const char *str, bool *is_success);
long long	ft_strtoll(char *num, bool *is_success);

/* put */
ssize_t		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);

/* mem */
void		ft_bzero(void *s, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_memset_ns(void *s, int c, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t len);
void		*ft_calloc(size_t count, size_t size);

/* list */
t_list		*ft_lstnew(void *content);
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
void		ft_lstadd_front(t_list **lst, t_list *new);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));

size_t		ft_lstsize(t_list *lst);

/* math */
size_t		minsize(size_t a, size_t b);
size_t		maxsize(size_t a, size_t b);
int			min_int(int a, int b);
int			max_int(int a, int b);

/* gnl */
char		*get_next_line(int fd, bool is_include_nl);
char		*strjoin_and_free_dst(char *dst, char *src);

void		*ft_free_gnl(char **alloc1, char **alloc2);

size_t		ft_strlen_gnl(const char *str);
size_t		cnt_chr_in_str(const char c, const char *str);
size_t		ft_strlcpy_gnl(char *dst, const char *src, size_t dstsize);

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

int			printf_controller(t_printf_info *info, const char *fmt, va_list *ap);
//int			printf_controller(t_printf_info *info, const char *fmt, ...);
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

#endif
