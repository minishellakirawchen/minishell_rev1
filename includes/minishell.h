/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:12:24 by takira            #+#    #+#             */
/*   Updated: 2023/01/22 22:52:09 by wchen            ###   ########.fr       */
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

# define STR_SPACE		"\t\n\v\f\r "
# define STR_QUOTE		"'\""
# define STR_OPERATOR	";&|<>()"
# define STR_SUBSHELL	"()"

# define CHR_SINGLE_QUOTE	'\''
# define CHR_DOUBLE_QUOTE	'\"'

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
typedef enum e_syntax_err	t_syntax_err;
typedef enum e_key_type		t_key_type;

/* ************************** */
/*           enum             */
/* ************************** */

enum e_token_type
{
	e_ope_semicolon = 0,		// ;
	e_ope_pipe = 1,				// |
	e_ope_or = 2,				// ||
	e_ope_and = 3,				// &&
	e_subshell_start = 4,		// (
	e_subshell_end = 5,			// )
	e_redirect_in = 6,			// <
	e_redirect_out = 7,			// >
	e_redirect_append = 8,		// >>
	e_heredoc = 9,				// <<
	e_file = 10,				//
	e_heredoc_eof = 11,			//
	e_word = 12,				//
	e_init = 13,				// init
	e_nothing = 20
};

enum e_syntax_err
{
	e_unexpected_token,
	e_newline,
};

enum e_key_type
{
	e_append,
	e_error,
	e_add,
};

/* ************************** */
/*          struct            */
/* ************************** */
// minishell info
struct s_info
{

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
	char			quote_chr;
	bool			is_quoted;
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
};

// ft_export
struct s_export_info
{
	t_env_elem		*elem;
	t_key_type		key_type;
	int				skip_flag;
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
int		command_execution(t_info *info);
int		execute_builtin(t_info *info, char **cmds);

/* ************************** */
/*       signal handler       */
/* ************************** */

/* ************************** */
/*         ft_builtin         */
/* ************************** */
int		ft_env(t_info *info);
int		ft_export(t_info *info, char **cmds);

/*         helper.c           */
void	*free_1d_alloc(void *alloc);
void	*free_info(t_info *info);
t_list	*get_envlist(void);
void	*perror_ret_nullptr(char *err);
void	free_env_elem(void *content);
void	free_token_elem(void *content);
void	print_key_value(void *content);
int		perror_and_return_int(char *err, int exit_status);
char	**get_elem(t_info *info, char *key);
int		set_elem(t_info *info, char *key, char *value);

void	debug_print_2d_arr(char **arr, char *str);
void	debug_print(const char *fmt,...);
void	debug_print_token_word(t_list *head, char *str);

int		is_tokentype_semicolon(t_token_type type);
int		is_tokentype_pipe_or_and(t_token_type type);
int		is_tokentype_redirection(t_token_type type);
int		is_tokentype_subshell(t_token_type type);


#endif //MINISHELL_H
