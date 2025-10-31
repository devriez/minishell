/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:46:09 by amoiseik          #+#    #+#             */
/*   Updated: 2025/10/20 16:53:17 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>     // access
#include <sys/stat.h>
#include <string.h>

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

static char	*get_cmd_path_inner(char *cmd_name, char **paths_arr)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths_arr[i])
	{
		full_path = join_path(paths_arr[i], cmd_name);
		if (!full_path)
			return (free_arr(paths_arr), NULL);
		if (access(full_path, F_OK) == 0)
			return (free_arr(paths_arr), full_path);
		free(full_path);
		i++;
	}
	return (free_arr(paths_arr), NULL);
}

char	*get_cmd_path(char *cmd_name, t_env *local_env)
{
	char	*paths_str;
	char	**paths_arr;

	if (strchr(cmd_name, '/'))
		return (ft_strdup(cmd_name));
	paths_str = get_path_from_env(local_env);
	if (!paths_str)
		return (NULL);
	paths_arr = ft_split(paths_str, ':');
	if (!paths_arr)
		return (NULL);
	return (get_cmd_path_inner(cmd_name, paths_arr));
}
