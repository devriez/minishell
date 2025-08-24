/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/24 20:51:47 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_from_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH", 4) == envp[i])
			return (envp[i] + ft_strlen("PATH="));
		i ++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd_name, char **envp)
{
	char	**paths;
	char	*full_path;
	char	*path_with_slash;
	int		i;

	i = 0;
	paths = ft_split(get_path_from_env(envp), ':');
	while (paths[i])
	{
		path_with_slash = ft_strjoin(paths[i], "/");
		if (!path_with_slash)
			return (NULL);
		full_path = ft_strjoin(path_with_slash, cmd_name);
		free(path_with_slash);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
		{
			free_arr(paths);
			return (full_path);
		}
		free(full_path);
		i ++;
	}
	return (free_arr(paths), NULL);
}


