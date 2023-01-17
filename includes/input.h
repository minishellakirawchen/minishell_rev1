/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:13:31 by takira            #+#    #+#             */
/*   Updated: 2023/01/17 17:45:30 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

#include "minishell.h"

# include <readline/readline.h>
# include <readline/history.h>

/*          typedef           */
typedef struct s_info	t_info;

/* ************************** */
/*           input            */
/* ************************** */
int	prompt_loop(t_info *info);


#endif //INPUT_H
