/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/11 13:45:09 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_to_list(char **envp)
{
	int		i;
	t_env	*local_env;

	local_env = NULL;
	i = 0;
	while (envp[i])
	{
		set_envv_from_str(&local_env, envp[i]);
		i++;
	}
	return (local_env);
}

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
