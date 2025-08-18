/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:44:52 by devriez           #+#    #+#             */
/*   Updated: 2025/05/20 12:25:10 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*s_res;
	size_t	len_s;
	size_t	reslen;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if ((size_t)start >= len_s)
		return (ft_strdup(""));
	if ((size_t)start + len > len_s)
		reslen = len_s - (size_t)start;
	else
		reslen = len;
	s_res = malloc(sizeof(char) * (reslen + 1));
	if (!s_res)
		return (NULL);
	ft_strlcpy(s_res, s + (size_t)start, reslen + 1);
	return (s_res);
}
