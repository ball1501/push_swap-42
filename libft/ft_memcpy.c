/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 16:30:28 by wngamkri          #+#    #+#             */
/*   Updated: 2026/08/29 16:30:29 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dst_ch;
	const unsigned char	*src_ch;
	size_t				i;

	if (!dst && !src)
		return (NULL);
	dst_ch = (unsigned char *)dst;
	src_ch = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		dst_ch[i] = src_ch[i];
		i++;
	}
	return (dst);
}
