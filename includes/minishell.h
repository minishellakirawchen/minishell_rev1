/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:12:24 by takira            #+#    #+#             */
/*   Updated: 2023/01/24 15:19:21 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>

# include "./../libs/include/libft.h"

# include "input.h"
# include "tokenizer.h"
# include "paeser.h"

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
/* struct */
typedef struct s_info			t_info;
typedef struct s_exec_list		t_exec_list;

/* lst->constent */
typedef struct s_env_elem		t_env_elem;
typedef struct s_token_elem		t_token_elem;
typedef struct s_split_info		t_split_info;
typedef struct s_command_list	t_command_list;
typedef struct s_redirect_list	t_redirect_list;

/* enum */
typedef enum e_token_type		t_token_type;
typedef enum e_node_kind		t_node_kind;

/* ************************** */
/*           enum             */
/* ************************** */

enum e_token_type
{
	e_semicolon = 0,		// ;
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
	e_nothing
};

enum e_node_kind
{
	e_node_head,			//root
	e_node_operator,
	e_node_subshell,
	e_node_pipeline,
	e_node_commands,
	e_node_init,
};

/* ************************** */
/*          struct            */
/* ************************** */
// minishell parameter
struct s_info
{
	t_list		*envlist_head;
	t_list		*tokenlist_head;
	t_exec_list	*execlist_head;
	char 		*readline_input;
};

// commands list for execute
struct s_exec_list
{
	// create_operator_list
	t_node_kind			node_kind;
	t_token_type		token_type; // tmp for print debug
	t_exec_list				*prev;
	t_exec_list				*next;

	// create_command_list
	t_list				*token_list_head;	// content=command_list, tmp_save
	t_list				*pipeline_commands;			//content=command_list
};


/*  lst->(void *)content  */
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
	ssize_t			depth; //TODO:depth
};

// split
struct s_split_info
{
	const char		*src;
	const char 		*delim;
	const char 		*setchars;
	bool			is_connect_to_next_word;  // hello"world"->[hello]=["world"]
	size_t			head_idx;
	size_t			word_len;
};

//
struct s_command_list
{
	t_node_kind		type; // command or subshell
	char 			**commands;
	t_list			*pipeline_token_list; //tmp_save, expansio後にchar **commandsへ整形する
	t_list			*subshell_token_list; //parsing create_operator_listから実行できる

	t_redirect_list	*redirect_list;
};

struct s_redirect_list
{
	t_token_type	type;
	char			*file;
	char			*heredoc_eof;
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
void	*free_info(t_info **info);
t_list	*get_envlist(void);
void	free_env_elem(void *content);
void	free_token_elem(void *content);
void	free_command_list_elem(void *content);
void	clear_exec_list(t_exec_list *exec_list);

/*         error_return.c           */
void	*perror_ret_nullptr(char *err);
int		perror_ret_int(char *err, int retno);


void	debug_print_2d_arr(char **arr, char *str);
void	debug_print(const char *fmt,...);
void	debug_print_token_word(t_list *head, char *str);
void	debug_print_tree(t_exec_list *root, char *str);
void	debug_print_exec_list(t_exec_list *head, char *str);

#endif //MINISHELL_H
