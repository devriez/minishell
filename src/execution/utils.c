/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:38:48 by amoiseik          #+#    #+#             */
/*   Updated: 2025/10/30 18:20:34 by amoiseik         ###   ########.fr       */
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

int	wait_exit_statuses(t_mini *mini)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	while (i < mini->pid_count)
	{
		waitpid(mini->pids[i], &status, 0);
		setup_signals(set_sigint);
		if (WIFEXITED(status))
			mini->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			mini->last_exit_status = 128 + WTERMSIG(status);
		i ++;
	}
	setup_signals(set_sigint);
	return (0);
}

void	close_all_other_pipes(int fd_in, int fd_out)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		if (fd != fd_in && fd != fd_out)
			close(fd);
		fd++;
	}
}

int	execute_helper(t_command *cmd, t_mini *mini,
									int fd_in, int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		if (fd_in != -1)
			close(fd_in);
		return (1);
	}
	if (fork_and_execute(cmd, mini, fd_in, pipe_fd[1]) == 1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		if (fd_in != -1)
			close(fd_in);
		return (1);
	}
	if (fd_in != -1)
		close(fd_in);
	close(pipe_fd[1]);
	return (0);
}

int	check_is_directory(const char *cmd_path, const char *cmd_name)
{
	struct stat	statbuf;

	if (stat(cmd_path, &statbuf) == -1)
		return (0);
	if (S_ISDIR(statbuf.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		return (126);
	}
	return (0);
}
