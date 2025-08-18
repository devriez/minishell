/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:16:01 by devriez           #+#    #+#             */
/*   Updated: 2025/05/19 14:51:49 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	j;
	size_t	d_size;
	size_t	s_size;

	d_size = ft_strlen(dst);
	s_size = ft_strlen(src);
	if (dstsize <= d_size)
		return (dstsize + s_size);
	j = 0;
	while ((d_size + j) < (dstsize - 1) && src[j] != '\0')
	{
		dst[d_size + j] = src[j];
		j++;
	}
	dst[d_size + j] = '\0';
	return (d_size + s_size);
}
