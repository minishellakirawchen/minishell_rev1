/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:15:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/08 20:50:34 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_EXECUTION_H
# define COMMAND_EXECUTION_H

# include "minishell.h"

/* fd */
# define READ			0
# define WRITE			1
# define FD_INFILE		0
# define FD_OUTFILE		1
# define FD_HEREDOC		2

/* g_signal_status */
# define CHDIR_FAILURE				1
# define EXIT_TOO_MANY_ARGS			1
# define FILE_OPEN_ERROR			1
# define CMD_NOT_FOUND				127
# define EXIT_SIGQUIT				131
# define EXIT_NUMERIC_ARGS_REQUIRED	255
# define SYNTAX_ERROR				258

/* string */
# define PATH				"PATH"
# define PATH_CURRENT		"./"
# define HEREDOC_TMP_FILE	".heredoc_tmp"

/* message */
# define ERRMSG_CMD_NOT_FOUND	"minishell: %s: command not found\n"
# define ERRMSG_NO_FILE		"minishell: %s: No such file or directory\n"
# define ERRMSG_NOT_DIR		"minishell: %s: Not a directory\n"

/* ************************** */
/*          typedef           */
/* ************************** */
typedef struct s_info			t_info;
typedef struct s_env_elem		t_env_elem;
typedef struct s_token_elem		t_token_elem;
typedef struct s_split_info		t_split_info;
typedef struct s_exec_list		t_exec_list;
typedef struct s_command_info	t_command_info;
typedef struct s_redirect_info	t_redirect_info;

typedef enum e_token_type		t_token_type;
typedef enum e_node_kind		t_node_kind;

typedef enum e_fopen_type		t_fopen;

/* ************************** */
/*           enum             */
/* ************************** */

enum e_fopen_type
{
	e_io_read,
	e_io_overwrite,
	e_io_append,
};

/* ************************** */
/*         function           */
/* ************************** */

/* execute_execlist.c */
int		execute_execlist(t_exec_list **execlist_head, t_info *info);
int		execute_pipeline(t_list_bdi *pipeline_cmds_head, t_info *info);

/* execute_pipeline.c */
int		execute_pipe_iter(t_list_bdi *pipeline, char **envp, t_info *info);

/* execute_subshell */
int		execute_subshell(t_list_bdi **token_list, t_info *info);

/* ft_exec.c */
int		ft_execve(\
t_command_info *command_info, char **minishell_envp, t_info *info);

/* judge_fork_process */
bool	is_child_process(pid_t pid);
bool	is_parent_process(pid_t pid);

/* pipe_helper.c */
void	init_pipefd_term(int prev_pipefd[2], int next_pipefd[2]);
void	copy_prevfd_to_newfd(int prev_pipefd[2], const int next_pipefd[2]);
int		dup2_fds(int prev_pipefd[2], int next_pipefd[2], t_list_bdi *next);
int		close_fds(int prev_pipefd[2], int next_pipefd[2], t_list_bdi *next);

/* create_envp.c */
char	**create_minishell_envp(t_list *envlist_head);

/* execute_redirect.c */
int		execute_redirect(t_command_info *command_info, t_info *info);

/* execute_heredoc.c */
int		execute_heredoc(t_exec_list **execlist_head);
bool	is_delimiter(const char *input_line, const char *delimiter);
bool	is_eof(char *line);
char	*get_heredoc_tmp_filename(int cnt);

/* open_file.c */
int		get_openfile_fd(const char *filename, t_fopen fopen_type);
int		get_io_fd(t_token_type io_type);
t_fopen	get_fopen_type(t_token_type io_type);

/* execute_builtin.c */
int		execute_builtin(t_info *info, char **cmds, bool in_pipe);
bool	is_builtin(char **cmds);
bool	is_single_builtin(t_list_bdi *pipeline_cmds_head);

#endif //COMMAND_EXECUTION_H
