/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:38:48 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/19 18:54:09 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**join_str_with_arr(char *str, char **arr)
{
	int		i;
	int		len;
	char	**new_arr;

	if (!arr || !str)
		return (NULL);
	len = 0;
	while (arr[len])
		len++;
	new_arr = malloc(sizeof(char *) * (len + 2));
	if (!new_arr)
		return (NULL);
	new_arr[0] = ft_strdup(str);
	if (!new_arr[0])
		return (free_arr(new_arr), NULL);
	i = 0;
	while (arr[i])
	{
		new_arr[i + 1] = ft_strdup(arr[i]);
		if (!new_arr[i + 1])
			return (free_arr(new_arr), NULL);
		i++;
	}
	new_arr[i + 1] = NULL;
	return (new_arr);
}
