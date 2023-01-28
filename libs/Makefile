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
GNL_DIR		= ./libgnl
PRINTF_DIR	= ./libftprintf
OBJ_DIR		= ./objs

# SRCS
LIBFT_SRC	= ft_strlen.c \
			  ft_strlcpy.c \
			  ft_strlcat.c \
			  ft_strchr.c \
			  ft_strrchr.c \
			  ft_strnstr.c \
			  ft_strncmp.c \
			  ft_bzero.c \
			  ft_memset.c \
			  ft_memcpy.c \
			  ft_memchr.c \
			  ft_memmove.c \
			  ft_memcmp.c \
			  ft_atoi.c \
			  ft_itoa.c \
			  ft_isalpha.c \
			  ft_isdigit.c \
		  	  ft_isalnum.c \
			  ft_isascii.c \
			  ft_isprint.c \
			  ft_isspace.c \
			  ft_isupper.c \
			  ft_islower.c \
			  ft_toupper.c \
			  ft_tolower.c \
			  ft_calloc.c \
			  ft_strdup.c \
			  ft_substr.c \
			  ft_strjoin.c \
			  ft_strtrim.c \
			  ft_split.c \
			  ft_split_set.c \
			  ft_strmapi.c \
			  ft_striteri.c\
			  ft_putchar_fd.c \
			  ft_putstr_fd.c \
			  ft_putendl_fd.c \
			  ft_putnbr_fd.c \
			  ft_lstnew.c \
			  ft_lstadd_front.c \
			  ft_lstsize.c \
			  ft_lstlast.c \
			  ft_lstadd_back.c \
			  ft_lstdelone.c \
			  ft_lstclear.c \
			  ft_lstiter.c \
			  ft_lstmap.c \
			  ft_math.c \
			  ft_strtoll.c \
			  is_in_str.c \

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
