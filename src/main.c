/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:47:11 by devriez           #+#    #+#             */
/*   Updated: 2025/09/03 18:45:01 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_exit_status = 0;

void	fork_and_execute(t_command *cmd, t_env *lockal_env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		printf("Error with forking!!!!!!");
		g_last_exit_status = 1;
		return ;
	}
	else if (pid == 0)
		exit(child_procces(cmd, lockal_env));
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_last_exit_status = WEXITSTATUS(status);
	else
		g_last_exit_status = 1;
}

void	redefine_and_close_fd(t_command *cmd, int *pipe_fd, int fd_0_last_pipe)
{
	if (fd_0_last_pipe)
	{
		dup2(fd_0_last_pipe, STDIN_FILENO);
		close(fd_0_last_pipe);
	}
	if (cmd->next)
	{
		fd_0_last_pipe = pipe_fd[0];
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

void	handle_multiply_cmds(t_command *cmd, t_env *lockal_env)
{

}

// void	handle_multiply_cmds(t_command *cmd, t_env *lockal_env)
// {
// 	int	pipe_fd[2];
// 	int	fd_0_last_pipe;

// 	fd_0_last_pipe == 0;
// 	while (cmd)
// 	{
// 		if (cmd->next)
// 		{
// 			if (pipe(pipe_fd) != -1)
// 				printf("Error with making pipe!!!!!");
// 		}
// 		redefine_and_close_fd(cmd, pipe_fd, fd_0_last_pipe);
// 		fork_and_execute(cmd, envp);
// 		cmd = cmd->next;
// 	}
// }

void	handle_single_cmd(t_command *cmd, t_env *lockal_env)
{
	char	*cmd_name;

	cmd_name = cmd->args[0];
	if (is_builtin(cmd_name))
		g_last_exit_status = execute_builtin(cmd, lockal_env);
	else
	{
		fork_and_execute(cmd, lockal_env);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_command	*cmd;
	t_env		*lockal_env;

	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	lockal_env = env_to_list(envp);
	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
		{
			printf("exit\n");
			exit(0);
		}
		cmd = johannes_func(line);
		if (cmd->next)
			handle_multiply_cmds(cmd, lockal_env);
		else
			handle_single_cmd(cmd, lockal_env);
	}
	return (0);
}
