/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:14:01 by takira            #+#    #+#             */
/*   Updated: 2023/01/19 15:14:50 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

/*          typedef           */
typedef struct s_info	t_info;


/* ************************** */
/*         tokenize           */
/* ************************** */

int		tokenize_input_line(t_info *info, const char *readline_input);
t_list	*get_delim_splitted_tokenlist(const char *src, const char *delim, const char *setchars);
int		split_by_operators(t_list **token_head);

int		validate_tokens_for_mandatory(t_info *info);
int		arrange_and_validate_token_list(t_list *tokenlist_head);


#endif //TOKENIZER_H
