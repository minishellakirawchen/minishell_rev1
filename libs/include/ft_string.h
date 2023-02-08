/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:57:14 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 13:31:26 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRING_H
# define FT_STRING_H

# include <stdlib.h>
# include <stdbool.h>

# include "ft_math.h"
# include "ft_std.h"

/* is */
int			ft_isupper(int c);
int			ft_islower(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
int			ft_isspace(char c);
int			ft_ispunct(char c);

bool		is_chr_in_str(char chr, const char *str);
bool		is_str1chrs_in_str2(const char *str1, const char *str2);

int			ft_toupper(int c);
int			ft_tolower(int c);

/* str */
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strncmp_ns(const char *s1, const char *s2, size_t n);
int			ft_strcmp_ns(const char *s1, const char *s2);
bool		is_same_str(const char *str1, const char *str2);

size_t		ft_strlen(const char *s);
size_t		ft_strlen_ns(const char *s);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t		ft_strlcat_ns(char *dst, const char *src, size_t dstsize);
size_t		get_arr_size(char **array);

char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strdup(const char *s1);
char		*ft_strdup_ns(const char *s1);
char		*ft_strtrim(char const *s1, char const *set);

char		**ft_split(char const *s, char c);
char		**ft_split_empty(char const *s, char c);

char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));

/* mem */
void		ft_bzero(void *s, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_memset_ns(void *s, int c, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t len);

#endif //FT_STRING_H
