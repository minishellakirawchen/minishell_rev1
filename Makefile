NAME	= minishell


# COMPILE FLAGS
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -MMD -MP

VPATH = $(SRC_DIR)


# SRCS
SRC_DIR	= ./srcs
SRC		= main.c \
		  input/input.c \
		  tokenizer/tokenizer.c \
		  parser/parser.c \
		  expansion/expansion.c \
		  command_execution/command_execution.c \
		  signal_handler/signal_handler.c \
		  ft_builtin/ft_echo.c \
		  helper/alloc_free.c \
		  ft_builtin/ft_echo.c

SRCS	= $(addprefix $(SRC_DIR)/ ,$(SRC))


# OBJS
OBJ_DIR	= ./objs
OBJ		= $(SRC:%.c=%.o)
OBJS	= $(addprefix $(OBJ_DIR)/, $(OBJ))


# DEPS
DEPS	= $(OBJS:%.o=%.d)


# LIBFT
LIBFT_DIR	= ./libs
LIBFT_NAME	= libft.a
LIBS		= $(addprefix $(LIBFT_DIR)/, $(LIBFT_NAME))


# LFLAGS
RL_DIR		= $(shell brew --prefix readline)/lib
LIBS_DIR	= $(LIBFT_DIR) $(RL_DIR)
LFLAGS		= $(addprefix -L, $(LIBS_DIR)) -lreadline


# IFLAGS
INC_DIR		= ./includes
RL_INC_DIR	= $(shell brew --prefix readline)/include
INCLUDES	= $(INC_DIR) $(RL_INC_DIR)
IFLAGS		= $(addprefix -I, $(INCLUDES))



# RULES
all		: $(NAME)

$(NAME)	: $(OBJS)
	@make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) $(LIBS) $(OBJS) -o $@

$(OBJ_DIR)/%.o	: %.c
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean	:
	@make clean -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)

fclean	: clean
	rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)

re		: fclean all

bonus	:

norm	:
	norminette -v
	norminette $(SRC_DIR) $(LIBFT_DIR) $(INC_DIR)

.PHONY	: all clean fclean re bonus norm

-include	$(DEPS)