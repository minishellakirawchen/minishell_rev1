/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:15:39 by takira            #+#    #+#             */
/*   Updated: 2023/02/06 00:24:21 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

# include <signal.h>

/* ************************** */
/*          typedef           */
/* ************************** */
typedef void	t_sigfunc(int);

/* ************************** */
/*          signal            */
/* ************************** */
void			init_sigaction(int sig_no, struct sigaction sig_act,
					t_sigfunc *sig_handler);
void			init_signal_execute(void);
int				print_signal_error(int exit_status, t_list_bdi *node,
					t_list_bdi *last_node);
int				do_heredoc(int fd, t_redirect_info *redirect_info,
					int *exit_status);

#endif //SIGNAL_HANDLER_H
