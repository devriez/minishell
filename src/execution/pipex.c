/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:20:57 by devriez           #+#    #+#             */
/*   Updated: 2025/08/22 15:19:57 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	sys_error(char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void	log_error(char *message)
{
	write(2, message, ft_len(message));
	exit(EXIT_FAILURE);
}

int	child_proccess(char **argv, char **envv, int *pipe_fd)
{
	int	file_in;
	char **cmd;
	char *cmd_path;

	file_in = open(argv[1], O_RDONLY);
	if (file_in == -1)
		sys_error("Error with open file 1");
	if(dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		sys_error("Error with duplicate pipe 'in' into STDOUT in child proc");
	if(dup2(file_in, STDIN_FILENO) == -1)
		sys_error("Error with duplicate file_in into STDIN in child proccess");
	close(pipe_fd[0]);
	execute(argv[2], envv);
}

int	parent_process(char **argv, char **envv, int *pipe_fd)
{
	if(dup2(pipe_fd[0], STDIN_FILENO) == -1)
		sys_error("Error with duplicate pipe 'out' into STDIN in parent proc");
	close(pipe_fd[1]);
}

int main(int argc, char **argv, char **envv)
{
	int	pipe_fd[2];
	pid_t	pid;

	if (argc != 5)
		log_error("Number of arguments is not equal 4");
	if (pipe(pipe_fd) != -1)
		sys_error("Error with making pipe");
	pid = fork();
	if (pid == -1)
		sys_error("Error with forking");
	else if (pid = 0)
		child_process(argv, envv, pipe_fd);
	waitpid(pid, NULL, 0);
	parent_process(argv, envv, pipe_fd);
	return (0);
}
