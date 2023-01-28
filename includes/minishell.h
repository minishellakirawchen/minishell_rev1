/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:12:24 by takira            #+#    #+#             */
/*   Updated: 2023/01/28 19:50:59 by takira           ###   ########.fr       */
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
# include "expansion.h"
# include "command_execution.h"

/* ************************** */
/*           macro            */
/* ************************** */
# define PROMPT	"minishell $> "
# define NL		"\n"

# define IFS	"\t\n\v\f\r "
# define STR_SPACE		"\t\n\v\f\r "
# define STR_QUOTE		"'\""
# define STR_OPERATOR	";&|<>()"
# define STR_SUBSHELL	"()"

# define STR_EXPAND_EXIT_STATUS	"$?"
# define STR_EXPANSION_DELIM	" !#$%&*+=./:=j?@[]{}^~'`\\\"" //ispunct include "_"

# define CHR_DOLLAR			'$'
# define CHR_QUESTION		'?'
# define CHR_SINGLE_QUOTE	'\''
# define CHR_DOUBLE_QUOTE	'\"'
# define CHA_PATH_DELIM		':'

/* loop */
# define FAILURE	0
# define SUCCESS	1
# define CONTINUE	2
# define BREAK		3

/* fd */
# define READ			0
# define WRITE			1
# define R_FD_INFILE	0
# define R_FD_OUTFILE	1
# define R_FD_HEREDOC	2

/* pid */
# define CHILD_PROCESS	0

/* exit_status */
# define CHDIR_FAILURE				1
# define EXIT_TOO_MANY_ARGS			1
# define FILE_OPEN_ERROR			1
# define CMD_NOT_FOUND				1
# define EXIT_SIGQUIT				131
# define EXIT_NUMERIC_ARGS_REQUIRED	255
# define SYNTAX_ERROR				258

/* string */
# define PATH	"PATH"


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
typedef struct s_command_info	t_command_info;
typedef struct s_redirect_info	t_redirect_info;

/* enum */
typedef enum e_token_type		t_token_type;
typedef enum e_node_kind		t_node_kind;

/* ************************** */
/*           enum             */
/* ************************** */

enum e_token_type
{
	e_semicolon = 0,		// ;
	e_ope_pipe = 1,			// |
	e_ope_or = 2,			// ||
	e_ope_and = 3,			// &&
	e_subshell_start = 4,	// (
	e_subshell_end = 5,		// )
	e_redirect_in = 6,		// <
	e_redirect_out = 7,		// >
	e_redirect_append = 8,	// >>
	e_heredoc = 9,			// <<
	e_file = 10,			//
	e_heredoc_eof = 11,		//
	e_word = 12,			//
	e_init = 13,			// init
	e_nothing
};

enum e_node_kind
{
	e_node_head,		// unused
	e_node_semicolon,
	e_node_and,
	e_node_or,
	e_node_operator,	// unused
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
	t_list_bdi	*tokenlist_head;
	t_exec_list	*execlist_head;
	char 		*readline_input;
	int			exit_status;
};

// commands list for execute
struct s_exec_list
{
	// create_operator_list
	t_node_kind		node_kind;
	t_token_type	token_type; // tmp for print debug
	t_exec_list		*prev;
	t_exec_list		*next;

	// use it execute_pipeline
	t_list_bdi		*pipeline_commands;	//content=command_list

	// create_command_list
	t_list_bdi		*token_list_head;	// content=command_list, tmp_save
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

// exec_list->pipeline_commands->content
struct s_command_info
{
	t_node_kind		type;			// command or subshell
	char 			**commands;		// argument of execve()
	t_list_bdi		*redirect_list;	// set fd before execute commands

	int				redirect_fd[3];	// in, out, here_doc;
	pid_t			pid;

	t_list_bdi		*pipeline_token_list; //tmp_save, expansio後にchar **commandsへ整形する
	t_list_bdi		*subshell_token_list; //parsing create_operator_listから実行できる
};

struct s_redirect_info
{
	t_token_type	io_type;
	char			*file;			// malloc
	char			*heredoc_eof;	// malloc
	bool			is_expansion;	// "eof"->not expand
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
void	**free_2d_alloc(void **alloc);
void	*free_info(t_info **info);
t_list	*get_envlist(void);
void	free_env_elem(void *content);
void	free_token_elem(void *content);
void	free_command_info(void *content);
void	free_redirect_info(void *content);
void	clear_exec_list(t_exec_list **exec_list);

/*         error_return.c           */
void	*perror_ret_nullptr(char *err);
int		perror_ret_int(char *err, int retno);


void	debug_print_2d_arr(char **arr, char *str);
void	debug_print(const char *fmt,...);
void	debug_print_tokens(t_list_bdi *head, char *str);
void	debug_print_tree(t_exec_list *root, char *str);
void	debug_print_exec_list(t_exec_list *head, char *str);
void	debug_print_exec_nodetype(t_exec_list *node);
void	debug_print_redirect_info(t_list_bdi *head, char *str);
void	debug_print_command_info(t_command_info *command_info);
//void	debug_print_command_info(t_command_info *command_info, bool subshell, bool token_cmds, bool cmds, bool redirect)

#endif //MINISHELL_H
