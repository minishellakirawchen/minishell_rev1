NAME	= minishell


# COMPILE FLAGS
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -MMD -MP
 #-g -fsanitize=address

VPATH = $(SRC_DIR)


# SRCS
SRC_DIR	= ./srcs
SRC		= main.c \
		  input/input.c \
		  input/judge_continue_loop.c \
		  input/set_tc_attr.c \
		  tokenizer/tokenizer.c \
		  tokenizer/space_split.c \
		  tokenizer/operator_split.c \
		  tokenizer/is_token_type.c \
		  tokenizer/validate_quote.c \
		  tokenizer/validate_syntax.c \
		  tokenizer/validate_operator.c \
		  tokenizer/check_operator_syntax.c \
		  tokenizer/arrange_and_validate_token.c \
		  tokenizer/create_token_elem.c \
		  tokenizer/create_splitted_word_token.c \
		  tokenizer/create_split_info.c \
		  tokenizer/get_trimmed_word.c \
		  tokenizer/get_split_before_after_opes.c \
		  parser/parser.c \
		  parser/create_operator_list.c \
		  parser/create_operator_list_helper.c \
		  parser/create_execlist_node.c \
		  parser/create_command_list.c \
		  parser/create_redirect_list.c \
		  parser/create_redirect_list_helper.c \
		  parser/get_filename_or_heredoc_eof.c \
		  parser/move_tokens_to_command_list.c \
		  parser/tree_operation.c \
		  parser/tree_helper.c \
		  expansion/expansion.c \
		  expansion/expand_in_tokens.c \
		  expansion/expand_in_filename.c \
		  expansion/expand_in_heredoc.c \
		  expansion/is_expand.c \
		  expansion/get_expanded_str.c \
		  expansion/create_expanded_commands.c \
		  expansion/concat_connected_tokens.c \
		  expansion/is_matches_wildcard.c \
		  expansion/expand_wildcard.c \
		  expansion/concat_wildcard_valid_list.c \
		  expansion/quote_removal_or_re_tokenize.c \
		  expansion/get_env_value.c \
		  expansion/get_dir_for_wildcard.c \
		  expansion/get_wildcard_token_from_dir.c \
		  expansion/sort_strlist_for_wildcard.c \
		  command_execution/execute_execlist.c \
		  command_execution/execute_pipeline_iter.c \
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
		  signal_handler/init_signal_execute.c \
		  signal_handler/init_signal_heredoc.c \
		  signal_handler/init_signal_prompt.c \
		  ft_builtin/ft_echo.c \
		  ft_builtin/ft_env.c \
		  ft_builtin/ft_export.c \
		  ft_builtin/ft_unset.c \
		  ft_builtin/ft_pwd.c \
		  ft_builtin/ft_cd.c \
		  ft_builtin/ft_exit.c \
		  helper/alloc_free.c \
		  helper/free_alloc_struct.c \
		  helper/get_envlist.c \
		  helper/error_return.c \
		  helper/print_key_value.c \
		  helper/builtin_judge.c \
		  helper/builtin_free.c \
		  helper/set_elem.c \
		  helper/get_elem.c \
		  helper/check_dir_exist.c \
		  helper/ft_lst_ascsort.c \
		  helper/chdir_setpath.c \
		  helper/update_shlvl.c \
		  helper/define_new_path.c \
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

bonus	: all

norm	:
	norminette -v
	norminette $(SRC_DIR) $(INC_DIR)

norm_lib	:
	norminette -v
	norminette $(LIBFT_DIR)

-include	$(DEPS)

.PHONY	: all clean fclean re bonus norm $(DEPS)
