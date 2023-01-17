/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:12:24 by takira            #+#    #+#             */
/*   Updated: 2023/01/17 18:31:30 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>

# include "./../libs/include/libft.h"
# include "input.h"

/* ************************** */
/*           macro            */
/* ************************** */
# define PROMPT	"minishell $> "
# define NL		"\n"

# define FAILURE	0
# define SUCCESS	1


/* ************************** */
/*          typedef           */
/* ************************** */
typedef struct s_info		t_info;
typedef struct s_env_elem	t_env_elem;



/* ************************** */
/*          struct            */
/* ************************** */
struct s_info
{
	int exit_status;
	t_list	*envlist;
};

struct s_env_elem
{
	char *key;
	char *value;
};

/* ************************** */
/*           enum             */
/* ************************** */





/* ************************** */
/*         tokenize           */
/* ************************** */

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
void debug_print_2d_arr(char **arr, char *str);

#endif //MINISHELL_H
