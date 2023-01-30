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
		  tokenizer/space_split.c \
		  tokenizer/operator_split.c \
		  tokenizer/is_token_type.c \
		  tokenizer/validate_quote.c \
		  tokenizer/validate_syntax.c \
		  tokenizer/validate_operator.c \
		  tokenizer/arrange_token.c \
		  parser/parser.c \
		  parser/create_operator_list.c \
		  parser/create_command_list.c \
		  parser/create_redirect_list.c \
		  parser/move_tokens_to_command_list.c \
		  parser/tree_operation.c \
		  parser/tree_helper.c \
		  expansion/expansion.c \
		  expansion/expand_var_in_tokens.c \
		  expansion/expand_var_in_heredoc.c \
		  expansion/is_expand.c \
		  expansion/get_expanded_str.c \
		  expansion/create_commands.c \
		  command_execution/execute_execlist.c \
		  command_execution/execute_pipeline.c \
		  command_execution/execute_redirect.c \
		  command_execution/execute_heredoc.c \
		  command_execution/execute_heredoc_helper.c \
		  command_execution/execute_subshell.c \
		  command_execution/create_envp.c \
		  command_execution/pipefd_handler.c \
		  command_execution/judge_fork_process.c \
		  command_execution/ft_exec.c \
		  command_execution/open_file.c \
		  command_execution/execute_builtin.c \
		  signal_handler/signal_handler.c \
		  ft_builtin/ft_echo.c \
		  ft_builtin/ft_env.c \
		  ft_builtin/ft_export.c \
		  ft_builtin/ft_unset.c \
		  ft_builtin/ft_pwd.c \
		  ft_builtin/ft_cd.c \
		  ft_builtin/ft_exit.c \
		  helper/alloc_free.c \
		  helper/get_envlist.c \
		  helper/error_return.c \
		  helper/print_key_value.c \
		  helper/builtin_judge.c \
		  helper/builtin_free.c \
		  helper/get_set_elem.c \
		  helper/check_dir_exist.c \
		  helper/ft_lst_ascsort.c \
		  helper/chdir_setpath.c \
		  debug/debug_print.c \

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
LFLAGS		= $(addprefix -L, $(LIBS_DIR)) -lreadline -lhistory


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
	norminette $(SRC_DIR) $(INC_DIR)

norm_lib	:
	norminette -v
	norminette $(LIBFT_DIR)

-include	$(DEPS)

.PHONY	: all clean fclean re bonus norm $(DEPS)
