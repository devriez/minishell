/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 20:58:05 by devriez           #+#    #+#             */
/*   Updated: 2025/09/11 21:42:46 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*res;
	size_t	len;

	len = ft_strlen(s1) + 1;
	res = (malloc(sizeof(char) * len));
	if (!res)
		return (perror("ft_strdup"), NULL);
	ft_strlcpy(res, s1, len);
	return (res);
}
