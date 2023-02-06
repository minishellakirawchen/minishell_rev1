/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:12:24 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 10:31:27 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./../libs/include/libft.h"
# include "command_execution.h"
# include "expansion.h"
# include "ft_builtin.h"
# include "input.h"
# include "parser.h"
# include "signal_handler.h"
# include "tokenizer.h"
# include <stdbool.h>
# include <stdio.h>

/* ************************** */
/*           macro            */
/* ************************** */
# define PROMPT "minishell $> "
# define NL "\n"

# define IFS "\t\n\v\f\r "
# define STR_SPACE "\t\n\v\f\r "
# define STR_QUOTE "'\""
# define STR_OPERATOR ";&|<>()"
# define STR_SUBSHELL "()"

# define STR_EXPAND_EXIT_STATUS "$?"
# define STR_EXPANSION_DELIM " !#$%&*+=./:=j?@[]{}^~'`\\\""

# define CHR_DOLLAR '$'
# define CHR_QUESTION '?'
# define CHR_SINGLE_QUOTE '\''
# define CHR_DOUBLE_QUOTE '\"'
# define CHA_PATH_DELIM ':'

/* loop */
# define FAILURE	0
# define SUCCESS	1
# define CONTINUE	-2
# define BREAK		-3

# define PROCESS_ERROR -1

# define EXIT_TOO_MANY_ARGS 1
# define EXIT_NUMERIC_ARGS_REQUIRED 255
# define SYNTAX_ERROR 258

/* ************************** */
/*          typedef           */
/* ************************** */
/* struct */
typedef struct s_info			t_info;
typedef struct s_exec_list		t_exec_list;
typedef struct s_gbl_var		t_gbl_var;

/* lst->constent */
typedef struct s_env_elem		t_env_elem;
typedef struct s_token_elem		t_token_elem;
typedef struct s_split_info		t_split_info;
typedef struct s_command_info	t_command_info;
typedef struct s_redirect_info	t_redirect_info;

/* enum */
typedef enum e_token_type		t_token_type;
typedef enum e_node_kind		t_node_kind;
typedef enum e_tokenlist_kind	t_list_kind;

enum							e_token_type
{
	e_semicolon = 0,
	e_ope_pipe = 1,
	e_ope_or = 2,
	e_ope_and = 3,
	e_subshell_start = 4,
	e_subshell_end = 5,
	e_redirect_in = 6,
	e_redirect_out = 7,
	e_redirect_append = 8,
	e_heredoc = 9,
	e_file = 10,
	e_heredoc_eof = 11,
	e_word = 12,
	e_init = 13,
	e_null
};

enum							e_node_kind
{
	e_node_head,
	e_node_semicolon,
	e_node_and,
	e_node_or,
	e_node_operator,
	e_node_subshell,
	e_node_pipeline,
	e_node_commands,
	e_node_init,
};

enum							e_list_kind
{
	e_pipeline_token_list,
	e_redirect_list,
};

/* ************************** */
/*          struct            */
/* ************************** */
/* minishell parameter */
struct							s_info
{
	t_list						*envlist_head;
	t_list_bdi					*tokenlist_head;
	t_exec_list					*execlist_head;
	char						*readline_input;
	int							exit_status;
};

/* commands list for execute */
struct							s_exec_list
{
	t_node_kind					node_kind;
	t_token_type				token_type;
	t_exec_list					*prev;
	t_exec_list					*next;
	t_list_bdi					*pipeline_commands;
	t_list_bdi					*token_list_head;
	t_list						*envlist_head;
	t_list						*tokenlist_head;
};

/*golbal variable for signal*/
struct							s_gbl_var
{
	volatile sig_atomic_t		heredoc_status;
	volatile sig_atomic_t		exit_status;
};

/*  lst->(void *)content  */
/* environment variable list */
struct							s_env_elem
{
	char						*key;
	char						*value;
	int							not_print;
};

/* token elem */
struct							s_token_elem
{
	char						*word;
	t_token_type				type;
	bool						is_connect_to_next;
	char						quote_chr;
	bool						is_quoted;
	int							subshell_depth;
	int							*wildcard_valid_flag;
};

/* split parameter */
struct							s_split_info
{
	size_t						head_idx;
	size_t						word_len;
	const char					*src;
	const char					*delim;
	const char					*setchars;
	bool						is_connect_to_next_word;
	bool						is_quoted;
	char						quote_chr;
};

/* exec_list->pipeline_commands->content */
/*
 * 	t_node_kind		type;			// command or subshell
	char 			**commands;		// argument of execve()
	t_list_bdi		*redirect_list;	// set fd before execute commands
	int				redirect_fd[3];	// in, out, here_doc;
	pid_t			pid;
	t_list_bdi		*pipeline_token_list; //tmp_save
	t_list_bdi		*subshell_token_list; //execute -> parsing
 */
struct							s_command_info
{
	t_node_kind					type;
	char						**commands;
	t_list_bdi					*redirect_list;
	int							redirect_fd[3];
	pid_t						pid;
	t_list_bdi					*pipeline_token_list;
	t_list_bdi					*subshell_token_list;
};

/* redirect_list->content */
struct							s_redirect_info
{
	t_token_type				io_type;
	char						*filename;
	char						*heredoc_eof;
	t_list_bdi					*token_list;
	bool						is_expansion;
	bool						is_ambiguous;
};

/* ************************** */
/*            main            */
/* ************************** */
void							prompt_int_handler(int sig_num);

/* ************************** */
/*           helper           */
/* ************************** */

/* helper.c */
t_list							*get_envlist(void);
t_env_elem						*create_new_envelem(char *key, char *value,
									int not_print);
void							print_key_value(void *content);

void							*free_1d_alloc(void *alloc);
void							**free_2d_alloc(void **alloc);
void							*free_info(t_info **info);
void							*free_allocs_ret_nullptr(void *alloc1,
									void *alloc2);

/* free_alloc_struct.c */
void							free_env_elem(void *content);
void							free_token_elem(void *content);
void							free_command_info(void *content);
void							free_redirect_info(void *content);
void							clear_exec_list(t_exec_list **exec_list);

int								update_shlvl(char **current_shlvl_str);
int								add_initial_shlvl(t_list **env_list_head);

/* error_return.c */
void							*perror_ret_nullptr(char *err);
int								perror_ret_int(char *err, int retno);
int								ambiguous_error(char *filename);
int								openfile_error(char *filename, char *strerror);

/* ************************** */
/*         debug print        */
/* ************************** */
void							debug_print_2d_arr(char **arr, char *str);
void							debug_print_tokens(t_list_bdi *head, char *str);
void							debug_print_exec_list(t_exec_list *head,
									char *str);
void							debug_print_redirect_info(t_list_bdi *head,
									char *str);
void							debug_print_command_info(
									t_command_info *command_info);
void							debug_print_wildcard_valid_list(int *list,
									size_t len);
void							debug_print_env(t_list *envlist);

#endif //MINISHELL_H
