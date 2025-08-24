/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:47:11 by devriez           #+#    #+#             */
/*   Updated: 2025/08/22 17:01:18 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_exit_status = 0;

t_command	johannes_func(char	*line)
{
	t_command	res;

	return res;
}

void	handle_signal(int	signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	
}

int	is_buildin(char *cmd)
{
	if (cmd == "echo" || cmd == "cd" || cmd == "pwd" || cmd == "export" ||
		cmd == "unset" || cmd == "env" || cmd == "exit")
		return (1);
	return (0);
}

int	handle_buildin(t_command *cmd)
{
	return (0);
}

void	child_procces(t_command *cmd, char ** envv, int *pipe_fd)
{
	
}

void	handle_single_coomand(t_command	*cmd, char **envp)
{
	
}

void	handle_commands(t_command	*cmd, char **envp)
{
	int			pipe_fd[2];
	int			fd_0_last_pipe;
	pid_t		pid;

	fd_0_last_pipe == 0;
	while(cmd->next)
	{
		if (pipe(pipe_fd) != -1)
			printf("Error with making pipe!!!!!"); // тут добавить ощищение и вывод ошибки
		if (fd_0_last_pipe != 0)
		{
			dup2(fd_0_last_pipe, STDIN_FILENO); //тут добавить, когда есть перенаправление
			close(fd_0_last_pipe);
		}	
		fd_0_last_pipe = pipe_fd[0];
		pid = fork();
		if (pid == 1)
			printf("Error with forking!!!!!!"); // тут добавить очищение и вывод ошибки
		else if (pid == 0)
			child_procces(cmd, envp, pipe_fd);
		waitpid(pid, NULL, 0);
	}
	if (fd_0_last_pipe != 0)
	{
		dup2(fd_0_last_pipe, STDIN_FILENO); //тут добавить, когда есть перенаправление
		close(fd_0_last_pipe);
		pid = fork();
		if (pid == 1)
			printf("Error with forking!!!!!!"); // тут добавить очищение и вывод ошибки
		else if (pid == 0)
			child_procces(cmd, envp, pipe_fd);
		waitpid(pid, NULL, 0);
	}	
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_command	cmd;
	
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
		{
			printf("exit\n");
			exit(0);
		}
		cmd = johannes_func(line);
		if (cmd.next)
			handle_multiply_cmds(&cmd, envp);
		else
			handle_single_cmd(&cmd, envp);
	}
	return (0);
}
		if (is_buildin(cmd.args[0]) && !cmd.next)
			g_last_exit_status = handle_buildin(cmds);