/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:38:48 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/28 14:24:56 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*johannes_func(char	*line)
{
    // Выделяем память для структуры на куче
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    
    // Выделяем память для массива указателей на строки
    cmd->args = malloc(sizeof(char *) * 5); // 4 элемента: "echo", "Hello", "World", NULL
    if (!cmd->args)
    {
        free(cmd);
        return NULL;
    }
    
    // Копируем каждую строку в новую, выделенную память
    cmd->args[0] = ft_strdup("echo");
	cmd->args[1] = ft_strdup("-n");
    cmd->args[2] = ft_strdup("Hello");
    cmd->args[3] = ft_strdup("World");
    cmd->args[4] = NULL;
    
    cmd->next = NULL;
    
    return cmd;
}

void	handle_signal(int signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

bool	is_internal(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || \
		ft_strcmp(cmd, "cd") == 0|| \
		ft_strcmp(cmd,"pwd")  == 0|| \
		ft_strcmp(cmd,"export") == 0|| \
		ft_strcmp(cmd, "unset") == 0|| \
		ft_strcmp(cmd, "env") == 0|| \
		ft_strcmp(cmd, "exit") == 0)
		return (true);
	return (false);
}

void	child_procces(t_command *cmd, char **envv, int *pipe_fd)
{

}

// char	*get_cmd_path(char *cmd_name, char **envp)
// {
// 	char	**paths;
// 	char	*full_path;
// 	char	*path_with_slash;
// 	int		i;

// 	i = 0;
// 	paths = ft_split(get_env_var(envp, "PATH"), ':');
// 	while (paths[i])
// 	{
// 		path_with_slash = ft_strjoin(paths[i], "/");
// 		if (!path_with_slash)
// 			return (NULL);
// 		full_path = ft_strjoin(path_with_slash, cmd_name);
// 		free(path_with_slash);
// 		if (!full_path)
// 			return (NULL);
// 		if (access(full_path, X_OK) == 0)
// 		{
// 			free_arr(paths);
// 			return (full_path);
// 		}
// 		free(full_path);
// 		i ++;
// 	}
// 	return (free_arr(paths), NULL);
// }
