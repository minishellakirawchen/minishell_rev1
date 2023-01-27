/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paeser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:14:22 by takira            #+#    #+#             */
/*   Updated: 2023/01/24 17:14:29 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PAESER_H
# define PAESER_H

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

/* create_operator_list.c */
//t_exec_list	*create_operator_list(t_list **tokenlist_head);
int	create_operator_list(t_info *info);


/* create_command_list.c */
int	create_command_list(t_exec_list **exec_list_head);

/* move_tokens_to_command_list.c */
void	move_tokens_to_command_list(t_list_bdi **token_list, t_command_info **command_list, t_list_bdi *popped_token);

void	add_top_of_tree(t_exec_list **tree, t_exec_list *add_elem);
void	add_bottom_of_tree(t_exec_list **tree, t_exec_list *add_elem);
t_exec_list	*get_last_node(t_exec_list *node);
t_exec_list	*pop_from_top(t_exec_list **tree);
t_exec_list	*pop_from_bottom(t_exec_list **tree);

int		parsing_token_list(t_info *info);


#endif //PAESER_H
