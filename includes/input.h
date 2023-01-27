/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:13:31 by takira            #+#    #+#             */
/*   Updated: 2023/01/23 18:43:56 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <stdio.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "minishell.h"

/*          typedef           */
typedef struct s_info	t_info;


/* ************************** */
/*           input            */
/* ************************** */
int		prompt_loop(t_info *info);
void	clear_input_info(t_info **info);

#endif //INPUT_H
