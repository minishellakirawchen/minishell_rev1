/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:14:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/05 13:50:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

/*          typedef           */
typedef struct s_info			t_info;
typedef struct s_token_elem		t_token_elem;
typedef struct s_split_info		t_split_info;

typedef enum e_token_type		t_token_type;


/* ************************** */
/*         tokenize           */
/* ************************** */

/* space_split.c */
t_list_bdi	*get_delim_splitted_tokenlist(const char *src, const char *delim, const char *setchars);

/* create_splitted_word_token.c */
t_token_elem	*create_splitted_word_token_from_src(t_split_info *s_info);

/* get_trimmed_word.c */
char *get_trimmed_word(t_split_info *s_info);


int			tokenize_input_line(t_info *info, const char *readline_input);
int			split_by_operators(t_list_bdi **token_head);

int			arrange_and_validate_token_list(t_list_bdi **tokenlist_head);
int			validate_quote(t_list_bdi *tokenlist_head);
int			validate_syntax_operators(t_list_bdi *tokenlist_head);;
int			validate_syntax_parenthesis_pairs(t_list_bdi *tokenlist_head);

int			validate_operator_tokens(t_token_elem *now_token, t_token_elem *next_token, bool is_head);

//t_token_elem	*create_token_elem(char *word);
t_token_elem	*create_token_elem(char *word, bool is_connected, bool is_quoted, char quote_chr);

t_split_info	*create_split_info(const char *src, const char *delim, const char *set);

/* validate_operator.c */
int		valid_control_operator(t_list_bdi **tokenlist_head);
int		validate_operator_sign(t_token_elem *now_token);

/* create_split_info.c */
t_split_info	*create_split_info(const char *src, const char *delim, const char *set);
void			init_split_info(t_split_info *s_info);

/* get_split_before_after_opes.c */
t_list_bdi	*get_split_before_after_opes(const char *src, const char *opes, char *quote);


int			is_tokentype_operator(t_token_type type);
int			is_tokentype_pipeline(t_token_type type);
int			is_tokentype_semicolon(t_token_type type);
int			is_tokentype_pipe_and_or(t_token_type type);
int			is_tokentype_redirection(t_token_type type);
int			is_tokentype_subshell(t_token_type type);
int			is_tokentype_io(t_token_type type);

#endif //TOKENIZER_H
