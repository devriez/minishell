/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:46:09 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/15 19:56:59 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_path_from_env(t_env *local_env)
{
	t_env	*env_var;

	env_var = local_env;
	while (env_var)
	{
		if (ft_strcmp(env_var->name, "PATH") == 0)
			return (env_var->value);
		env_var = env_var->next;
	}
	return (NULL);
}

static char	*join_path(const char *path, const char *filename)
{
	char	*temp;
	char	*full_path;

	if (!path || !filename)
		return (NULL);
	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, filename);
	if (!full_path)
	{
		free(temp);
		return (NULL);
	}
	free(temp);
	return (full_path);
}

char	*get_cmd_path(char *cmd_name, t_env *local_env)
{
	char	*paths_str;
	char	**paths_arr;
	char	*full_path;
	int		i;

	// Check if it's an absolute path or contains '/'
	if (cmd_name[0] == '/' || ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, X_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	
	i = 0;
	paths_str = get_path_from_env(local_env);
	if (!paths_str)
		return NULL;
	paths_arr = ft_split(paths_str, ':');
	if (!paths_arr)
		return (NULL);
	while (paths_arr[i])
	{
		full_path = join_path(paths_arr[i], cmd_name);
		if (!full_path)
			return (free_arr(paths_arr), NULL);
		if (access(full_path, X_OK) == 0)
			return (free_arr(paths_arr), full_path);
		free(full_path);
		i ++;
	}
	return (free_arr(paths_arr), NULL);
}
