/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:06 by devriez           #+#    #+#             */
/*   Updated: 2025/08/04 23:27:10 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i ++;
	}
	free(arr);
}

char	*get_PATH(char **envv)
{
	int	i;
	
	i = 0;
	while (envv[i])
	{
		if (ft_strnstr(envv[i], "PATH", 4) == envv[i])
			return (envv[i] + ft_strlen("PATH="));
		i ++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd_name, char **envv)
{
	char	**paths;
	char	*full_path;
	char	*path_with_slash;
	int		i;

	i = 0;
	paths = ft_split(get_PATH(envv), ":");
	while (paths[i])
	{
		path_with_slash = ft_strjoin(paths[i], "/");
		if (!path_with_slash)
			return (NULL);
		full_path = ft_strjoin(path_with_slash, cmd_name);
		free(path_with_slash);
		if (!full_path)
			return(NULL);
		if (access(full_path, X_OK))
		{
			free_arr(paths);
			return (full_path);
		}
		free(full_path);
		i ++;
	}
	return (free_arr(paths), NULL);
}

void execute(char *cmd_from_input, char **envv)
{
	char **cmd;
	char **cmd_path;
	
	cmd = ft_split(cmd_from_input, ' ');
	cmd_path = get_cmd_path(cmd[0], envv);
	if (!cmd_path)
	{
		free_arr(cmd);
		log_error('command is not executable');
	}
	if(execve(cmd_path, cmd, envv) == -1);
	{
		free(cmd_path);
		free_arr(cmd);
		sys_error("Error with execve");
	}
}
