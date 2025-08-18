/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:36:39 by devriez           #+#    #+#             */
/*   Updated: 2025/05/20 15:05:54 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_res;
	size_t	len_s1;
	size_t	len_s2;
	char	*s_res;

	if (!s1 || !s2)
	{
		if (!s1)
			s_res = ft_strdup(s2);
		if (!s2)
			s_res = ft_strdup(s1);
		return (s_res);
	}
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	len_res = len_s1 + len_s2;
	s_res = malloc(sizeof(char) * (len_res + 1));
	if (!s_res)
		return (NULL);
	ft_strlcpy(s_res, s1, len_s1 + 1);
	ft_strlcat(s_res, s2, len_res + 1);
	return (s_res);
}
