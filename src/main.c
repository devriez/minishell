/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:47:11 by devriez           #+#    #+#             */
/*   Updated: 2025/08/29 16:24:48 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_exit_status = 0;

void	fork_and_execute(t_command *cmd, t_env *lockal_envp)
{
	
}

// void	fork_and_execute(t_command *cmd, t_env *lockal_envp)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == -1)
// 		printf("Error with forking!!!!!!");
// 	else if (pid == 0)
// 		child_procces(cmd, envp);
// 	waitpid(pid, NULL, 0);
// }

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

void	handle_multiply_cmds(t_command *cmd, t_env *lockal_envp)
{
	
}

// void	handle_multiply_cmds(t_command *cmd, t_env *lockal_envp)
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

void	handle_single_cmd(t_command *cmd, t_env *lockal_envp)
{
	char	*cmd_name;

	cmd_name = cmd->args[0];
	if (is_builtin(cmd_name))
			g_last_exit_status = execute_internal(cmd, lockal_envp);
	else
	{
		fork_and_execute(cmd, lockal_envp);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_command	*cmd;
	t_env		*lockal_envp;

	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	lockal_envp = env_to_list(envp);
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
			handle_multiply_cmds(cmd, lockal_envp);
		else
			handle_single_cmd(cmd, lockal_envp);
	}
	return (0);
}
