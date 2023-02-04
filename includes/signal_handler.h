/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:15:39 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 01:28:24 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

#include <signal.h>

typedef void sigfunc(int);

void	init_sigaction(int sig_no, struct sigaction sig_act, sigfunc *sig_handler);
void	init_signal_execute(void);
void	init_signal_prompt(void);
int		print_signal_error(int exit_status, t_list_bdi *node, t_list_bdi *last_node);

#endif //SIGNAL_HANDLER_H
