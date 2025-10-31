/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 12:05:48 by johartma          #+#    #+#             */
/*   Updated: 2025/10/29 18:50:29 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	expand_alt(size_t *i, char **result, char *original)
{
	char	*temp;

	temp = ft_strjoin_char(*result, original[*i]);
	free(*result);
	if (!temp)
	{
		*result = NULL;
		return ;
	}
	*result = temp;
	(*i)++;
}

static int	numlen(pid_t n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1; 
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*pid_to_array(pid_t n)
{
	char	*res;
	int		len;

	len = numlen(n);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (n == 0)
		res[0] = '0';
	while (n > 0)
	{
		res[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (res);
}
