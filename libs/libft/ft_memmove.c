/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:36:10 by takira            #+#    #+#             */
/*   Updated: 2023/01/20 16:44:43 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*dst_cpy;
	const unsigned char	*src_cpy;
	size_t				i;
	size_t				j;

	dst_cpy = (unsigned char *) dst;
	src_cpy = (const unsigned char *) src;
	i = 0;
	j = 0;
	if (dst < src)
		while (len--)
			dst_cpy[i++] = src_cpy[j++];
	else if (dst > src)
		while (len--)
			dst_cpy[len] = src_cpy[len];
	return (dst);
}
