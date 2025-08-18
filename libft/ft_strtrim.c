/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:42:11 by devriez           #+#    #+#             */
/*   Updated: 2025/05/20 12:52:07 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set( char const c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set ++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	finish;

	if (!set)
		return (ft_strdup(s1));
	if (!s1)
		return (ft_strdup(""));
	if (ft_strlen(s1) == 0)
		return (ft_strdup(""));
	start = 0;
	finish = ft_strlen(s1) - 1;
	while (is_set(s1[start], set))
		start ++;
	while (finish != 0 && is_set(s1[finish], set))
		finish --;
	if (start > finish)
		return (ft_strdup(""));
	return (ft_substr(s1, start, finish - start + 1));
}
