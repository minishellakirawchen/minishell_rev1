/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:57:14 by takira            #+#    #+#             */
/*   Updated: 2023/01/26 18:09:10 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LIST_H
# define FT_LIST_H

# include <stdlib.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_list_bdi
{
	void				*content;
	struct s_list_bdi	*prev;
	struct s_list_bdi	*next;
}						t_list_bdi;

/* list */
t_list		*ft_lstnew(void *content);
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

void		ft_lstadd_front(t_list **lst, t_list *new);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));

size_t		ft_lstsize(t_list *lst);

/* list bidirectional */
t_list_bdi	*ft_lstnew_bdi(void *content);
t_list_bdi	*ft_lstpop_bdi(t_list_bdi **lst);
t_list_bdi	*ft_lstlast_bdi(t_list_bdi *lst);

void		ft_lstadd_front_bdi(t_list_bdi **lst, t_list_bdi *new);
void		ft_lstadd_back_bdi(t_list_bdi **lst, t_list_bdi *new);
void		ft_lstdelone_bdi(t_list_bdi **lst, void (*del)(void *));
void		ft_lstclear_bdi(t_list_bdi **lst, void (*del)(void *));
void		ft_lstiter_bdi(t_list_bdi *lst, void (*f)(void *));

size_t		ft_lstsize_bdi(t_list_bdi *lst);

#endif //FT_LIST_H
