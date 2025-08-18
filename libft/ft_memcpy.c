/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:23:17 by devriez           #+#    #+#             */
/*   Updated: 2025/05/19 14:55:13 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dst_tmp;
	const unsigned char	*src_tmp;
	size_t				i;

	if (!dst && !src)
		return (NULL);
	i = 0;
	dst_tmp = (unsigned char *)dst;
	src_tmp = (const unsigned char *)src;
	while (i < n)
	{
		dst_tmp[i] = src_tmp[i];
		i ++;
	}
	return (dst);
}
