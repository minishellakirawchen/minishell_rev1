/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:14:46 by takira            #+#    #+#             */
/*   Updated: 2023/01/30 10:44:19 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"

/* ************************** */
/*          typedef           */
/* ************************** */
typedef struct s_info			t_info;
typedef struct s_env_elem		t_env_elem;
typedef struct s_token_elem		t_token_elem;
typedef struct s_split_info		t_split_info;
typedef struct s_exec_list		t_exec_list;
typedef struct s_command_info	t_command_info;

typedef enum e_token_type		t_token_type;
typedef enum e_syntax_err		t_syntax_err;
typedef enum e_node_kind		t_node_kind;


/* ************************** */
/*           enum             */
/* ************************** */

/* ************************** */
/*          struct            */
/* ************************** */

/* ************************** */
/*         function           */
/* ************************** */

/* expansion.c */
int		expand_var_and_create_commands_from_tokens(t_exec_list **pipeline, t_info *info);


/* expand_var_in_tokens.c */
int		expansion(t_info *info);//test
int		remove_quote_in_tokens(t_list_bdi **list_head);


/* expand_var_in_heredoc.c */
int	do_expansion_in_heredoc(char *filename, t_info *info)


/* create_commands.c */
int		expand_var_in_cmd_and_create_cmds_from_tokens(t_command_info **cmd_list, t_info *info);

char	*concat_tokens(t_list_bdi *list_head);
int		expand_var_in_tokens(t_list_bdi **list_head, t_info *info);


int		remove_quotes(char **token_word);
char	*get_expanded_str(char *src, t_info *info);

/* get_expanded_str.c */
char	*get_expanded_str(char *src, t_info *info);
char	*concat_dst_to_src(char **dst, char *src);
char	*get_name_str(const char *str_start_with_dollar);
char	*get_env_value(const char *search_key, t_list *env_list_head);
int		expand_exit_status(char **expanded_str, int exit_status);


/* is_expand.c */
bool	is_expandable_var_in_str(const char *str, char quote_chr);
bool	is_name(const char *str);
bool	is_str_expandable_name(const char *str);
bool	is_expandable_exit_status(const char *str);
bool	is_expandable_str_with_dollar(const char *str);


#endif //EXPANSION_H
