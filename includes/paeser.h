/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paeser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:14:22 by takira            #+#    #+#             */
/*   Updated: 2023/01/20 21:13:55 by takira           ###   ########.fr       */
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
typedef struct s_tree			t_tree;


typedef enum e_token_type		t_token_type;
typedef enum e_syntax_err		t_syntax_err;
typedef enum e_tree_node_type	t_tree_node_type;


/* ************************** */
/*           enum             */
/* ************************** */

enum e_tree_node_type
{
	e_root,
	e_operator,
	e_subshell,
	e_command_line,
};

/* ************************** */
/*          struct            */
/* ************************** */

struct s_tree
{
	t_tree_node_type	tree_type;
	t_token_elem		*token;
	t_list				*token_list_head; //cmds

	t_tree				*left;
	t_tree				*right;
};

#endif //PAESER_H
