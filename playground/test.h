/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 20:20:39 by takira            #+#    #+#             */
/*   Updated: 2023/01/31 16:30:13 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
#define TEST_H

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <signal.h>

# include "../libs/include/libft.h"

# define READ	0
# define WRITE	1

# define FAIL	0
# define PASS	1

# define max(a, b)	(a >= b ? a : b)

# define BUFFER	256




#endif
