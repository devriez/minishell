/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:38:48 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/18 18:06:08 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	signal(SIGINT, handle_signal);
}

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
