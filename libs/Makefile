# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/16 09:32:11 by takira            #+#    #+#              #
#    Updated: 2023/01/29 01:24:13 by wchen            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libft.a

# CC CFLAGS
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -MMD -MP

# DIRS
LIBFT_DIR	= ./libft
GNL_DIR		= ./libft/gnl
PRINTF_DIR	= ./libft/printf
OBJ_DIR		= ./objs

# SRCS
LIBFT_SRC	= string/str/ft_strlen.c \
			  string/str/ft_strlcpy.c \
			  string/str/ft_strlcat.c \
			  string/str/ft_strchr.c \
			  string/str/ft_strrchr.c \
			  string/str/ft_strnstr.c \
			  string/str/ft_strncmp.c \
			  string/str/ft_strcmp.c \
			  string/str/ft_isalpha.c \
			  string/str/ft_isdigit.c \
		  	  string/str/ft_isalnum.c \
			  string/str/ft_isascii.c \
			  string/str/ft_isprint.c \
			  string/str/ft_isspace.c \
			  string/str/ft_isupper.c \
			  string/str/ft_islower.c \
			  string/str/ft_toupper.c \
			  string/str/ft_tolower.c \
			  string/str/ft_strdup.c \
			  string/str/ft_substr.c \
			  string/str/ft_strjoin.c \
			  string/str/ft_strtrim.c \
			  string/str/ft_split.c \
			  string/str/ft_split_emp.c \
			  string/str/ft_strmapi.c \
			  string/str/ft_striteri.c\
			  string/str/ft_ispunct.c \
			  string/str/is_in_str.c \
			  string/str/is_same_str.c \
			  string/str/get_array_size.c \
			  string/mem/ft_bzero.c \
			  string/mem/ft_memset.c \
			  string/mem/ft_memcpy.c \
			  string/mem/ft_memchr.c \
			  string/mem/ft_memmove.c \
			  string/mem/ft_memcmp.c \
			  std/ft_calloc.c \
			  std/ft_atoi.c \
			  std/ft_itoa.c \
			  std/ft_strtoll.c \
			  put/ft_putchar_fd.c \
			  put/ft_putstr_fd.c \
			  put/ft_putendl_fd.c \
			  put/ft_putnbr_fd.c \
			  math/ft_math.c \
			  list/ft_lstnew.c \
			  list/ft_lstadd_front.c \
			  list/ft_lstsize.c \
			  list/ft_lstlast.c \
			  list/ft_lstadd_back.c \
			  list/ft_lstdelone.c \
			  list/ft_lstclear.c \
			  list/ft_lstiter.c \
			  list/ft_lstmap.c \
			  list/ft_lstnew_bdi.c \
			  list/ft_lstadd_back_bdi.c \
			  list/ft_lstadd_front_bdi.c \
			  list/ft_lstclear_bdi.c \
			  list/ft_lstdelone_bdi.c \
			  list/ft_lstiter_bdi.c \
			  list/ft_lstlast_bdi.c \
			  list/ft_lstsize_bdi.c \
			  list/ft_lstnew_bdi.c \
			  list/ft_lstpop_bdi.c

GNL_SRC		= get_next_line.c \
              get_next_line_utils.c

PRINTF_SRC	= ft_printf.c \
			  printf_controller.c \
			  printf_utils.c \
			  ft_put_for_printf.c \
			  input_printf_format.c \
			  vaild_info_for_each_fmt.c \
			  print_num.c \
			  print_string.c

SRCS		= $(addprefix $(LIBFT_DIR)/, $(LIBFT_SRC)) \
			  $(addprefix $(GNL_DIR)/, $(GNL_SRC)) \
			  $(addprefix $(PRINTF_DIR)/, $(PRINTF_SRC))

# OBJS
OBJ			= $(SRCS:%.c=%.o)
OBJS		= $(addprefix $(OBJ_DIR)/, $(OBJ))

# DEPS
DEPS		= $(SRCS:%.c=%.d)

# INCLUDES
INCLUDE_DIR	= ./include
IFLAGS		= $(addprefix -I, $(INCLUDE_DIR))


# RULES
$(NAME)	: $(OBJS)
	ar rcs $(NAME) $(OBJS)
	#$(CC) $(CFLAGS) $(IFAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o	:%.c
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

all	: $(NAME)

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re	: fclean all

.PYONY : all clean fclean re

-include	$(DEPS)
