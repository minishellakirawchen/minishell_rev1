/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_ascsort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 11:02:01 by wchen             #+#    #+#             */
/*   Updated: 2023/01/29 16:11:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	front_back_split(t_list *source, t_list **front_ref,
		t_list **back_ref)
{
	t_list	*fast;
	t_list	*slow;

	slow = source;
	fast = source->next;
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			fast = fast->next;
			slow = slow->next;
		}
	}
	*front_ref = source;
	*back_ref = slow->next;
	slow->next = NULL;
}

static ssize_t	sort_max_size(t_env_elem *a, t_env_elem *b)
{
	ssize_t	a_len;
	ssize_t	b_len;

	a_len = ft_strlen_ns(a->key);
	b_len = ft_strlen_ns(b->key);
	if (a_len > b_len)
		return (a_len);
	else
		return (b_len);
}

static t_list	*sorted_merge(t_list *a, t_list *b)
{
	t_list	*result;

	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);
	if (ft_strncmp(((t_env_elem *)a->content)->key,
			((t_env_elem *)b->content)->key,
			sort_max_size(a->content, b->content)) <= 0)
	{
		result = a;
		result->next = sorted_merge(a->next, b);
	}
	else
	{
		result = b;
		result->next = sorted_merge(a, b->next);
	}
	return (result);
}

void	ft_lst_ascsort(t_list **headRef)
{
	t_list	*head;
	t_list	*a;
	t_list	*b;

	head = *headRef;
	if ((head == NULL) || (head->next) == NULL)
		return ;
	front_back_split(head, &a, &b);
	ft_lst_ascsort(&a);
	ft_lst_ascsort(&b);
	*headRef = sorted_merge(a, b);
}
