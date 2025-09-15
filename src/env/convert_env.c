/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/15 19:35:17 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	count_env_vars(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

static char	**fill_env_array(t_env *env_list, int count)
{
	char	**env_array;
	int		i;
	t_env	*current;
	char	*temp;

	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		temp = ft_strjoin(current->name, "=");
		if (!temp)
			return (free_arr(env_array), NULL);
		env_array[i] = ft_strjoin(temp, current->value);
		free(temp);
		if (!env_array[i])
			return (free_arr(env_array), NULL);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	**env_to_char_array(t_env *env_list)
{
	int		count;
	char	**env_array;

	if (!env_list)
	{
		env_array = (char **)malloc(sizeof(char *));
		if (env_array)
			env_array[0] = NULL;
		return (env_array);
	}
	count = count_env_vars(env_list);
	env_array = fill_env_array(env_list, count);
	if (!env_array)
		return (NULL);
	return (env_array);
}
