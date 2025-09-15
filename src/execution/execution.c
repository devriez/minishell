/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:46:13 by devriez           #+#    #+#             */
/*   Updated: 2025/09/11 21:27:50 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fork_and_execute(t_command *cmd, t_env *local_env, int fd_in, int fd_out)
{
	pid_t pid;
	int status;
	int exit_status;

	pid = fork();
	if (pid == -1)
		return (printf("Error with forking\n"), 1);
	if (pid == 0)
	{
		child_setup(fd_in, fd_out);
		exit(child_process(cmd, local_env));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else
		exit_status = 1;
	return (exit_status);
}

static int	execute_pipeline_cmd(t_command *cmd, t_env *local_env,
		int fd_in, int *pipe_fd, int is_last)
{
	int exit_status;

	if (!is_last)
	{
		if (pipe(pipe_fd) == -1)
		{
			printf("Error with making pipe\n");
			if (fd_in != -1)
				close(fd_in);
			return (1);
		}
	}
	if (!is_last)
		exit_status = fork_and_execute(cmd, local_env, fd_in, pipe_fd[1]);
	else
		exit_status = fork_and_execute(cmd, local_env, fd_in, STDOUT_FILENO);
	if (fd_in != -1)
		close(fd_in);
	if (!is_last)
		close(pipe_fd[1]);
	return (exit_status);
}

void	handle_multiply_cmds(t_command *cmd, t_env *local_env)
{
	t_command	*current;
	int			fd_in;
	int			pipe_fd[2];

	fd_in = -1;
	current = cmd;
	while (current)
	{
		if (current->next)
			g_last_exit_status = execute_pipeline_cmd(
				current, local_env, fd_in, pipe_fd, 0);
		else
			g_last_exit_status = execute_pipeline_cmd(
				current, local_env, fd_in, pipe_fd, 1);
		if (current->next)
			fd_in = pipe_fd[0];
		current = current->next;
	}
}

void	handle_single_cmd(t_command *cmd, t_env *local_env)
{
	int	saved_stdin;
	int	saved_stdout;

	if (is_builtin(cmd->name))
	{
		if (cmd->redirections)
		{
			if (redirect_in_parent(cmd->redirections, \
									&saved_stdin, \
									&saved_stdout))
			{
				g_last_exit_status = 1;
				return ;
			}
		}
		g_last_exit_status = execute_builtin(cmd, local_env);
		if (cmd->redirections)
			restore_parent_io(saved_stdin, saved_stdout);
	}
	else
		g_last_exit_status = fork_and_execute(cmd, local_env, \
												STDIN_FILENO, \
												STDOUT_FILENO);
}
