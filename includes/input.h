/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:13:31 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 10:11:45 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <stdio.h>
# include <termios.h>
# include <unistd.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "minishell.h"

/* ************************** */
/*          typedef           */
/* ************************** */
typedef struct s_info	t_info;

/* ************************** */
/*           input            */
/* ************************** */
int		prompt_loop(t_info *info);
void	clear_input_info(t_info **info);
int		set_tc_attr_out_execute(void);
int		set_tc_attr_in_execute(void);
bool	is_eof_exit(int process_exit_value);
bool	is_minishell_abort(int process_exit_value);

#endif //INPUT_H
