/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:12:24 by takira            #+#    #+#             */
/*   Updated: 2023/01/18 16:47:42 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>

# include "./../libs/include/libft.h"

# include "input.h"
# include "tokenizer.h"

/* ************************** */
/*           macro            */
/* ************************** */
# define PROMPT	"minishell $> "
# define NL		"\n"

# define STR_SPACE	"\t\n\v\f\r "
# define STR_QUOTE	"'\""

# define FAILURE	0
# define SUCCESS	1

# define DEBUG		1

/* ************************** */
/*          typedef           */
/* ************************** */
typedef struct s_info		t_info;
typedef struct s_env_elem	t_env_elem;
typedef struct s_token_elem	t_token_elem;
typedef struct s_split_info	t_split_info;
typedef enum e_token_type	t_token_type;

/* ************************** */
/*           enum             */
/* ************************** */

enum e_token_type
{
	e_init,
	e_operator,
	e_word,
	e_redirection,
};

/* ************************** */
/*          struct            */
/* ************************** */
// minishell info
struct s_info
{
	int		exit_status;
	t_list	*envlist_head;
	t_list	*tokenlist_head;
};

// environment variable list
struct s_env_elem
{
	char *key;
	char *value;
};

// token elem
struct s_token_elem
{
	char			*word;
	t_token_type	type;
	bool			is_connect_to_next_word;
};

// split
struct s_split_info
{
	const char		*src;
	const char 		*delim;
	const char 		*setchars;
	bool			is_connect_to_next_word;  // hello"world"
	size_t			head_idx;
	size_t			word_len;
	char 			setchr;
};


/* ************************** */
/*          parsing           */
/* ************************** */

/* ************************** */
/*         expansion          */
/* ************************** */

/* ************************** */
/*     command execution      */
/* ************************** */

/* ************************** */
/*       signal handler       */
/* ************************** */

/* ************************** */
/*         ft_builtin         */
/* ************************** */


/*         helper.c           */
void	*free_1d_alloc(void *alloc);
void	*free_info(t_info *info);
t_list	*get_envlist(void);
void	*perror_ret_nullptr(char *err);
void	free_env_elem(void *content);
void	free_token_elem(void *content);


void	debug_print_2d_arr(char **arr, char *str);
void	debug_print(const char *fmt,...);


#endif //MINISHELL_H
