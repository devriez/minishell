/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:46:13 by devriez           #+#    #+#             */
/*   Updated: 2025/09/22 17:29:46 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fork_and_execute(t_command *cmd, \
							t_mini *mini, \
							int fd_in, \
							int fd_out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		printf("Error with forking\n");
		return (1);
	}
	if (pid == 0)
	{
		child_setup(fd_in, fd_out);
		exit(child_process(cmd, mini->env));
	}
	setup_signals(ignore_sigint_print_newline);
	mini->pids[mini->pid_count] = pid;
	mini->pid_count++;
	return (0);
}

static int	wait_exit_statuses(t_mini *mini)
{
	int	status;
	int	i;

	i = 0;
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

void	handle_single_cmd(t_command *cmd, t_mini *mini)
{
	int	saved_stdin;
	int	saved_stdout;

	if (is_builtin(cmd->name))
	{
		if (cmd->redirections)
		{
			if (redirect_in_parent(cmd->redirections, &saved_stdin, \
															&saved_stdout))
			{
				mini->last_exit_status = 1;
				return ;
			}
		}
		mini->last_exit_status = execute_builtin(cmd, mini->env);
		if (cmd->redirections)
			restore_parent_io(saved_stdin, saved_stdout);
	}
	else
	{
		if (fork_and_execute(cmd, mini, STDIN_FILENO, STDOUT_FILENO) == 1)
			mini->last_exit_status = 1;
		else
			wait_exit_statuses(mini); 
	}
}

static int	execute_pipeline_cmd(t_command *cmd, t_mini *mini,
									int fd_in, int *pipe_fd)
{
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			printf("Error with making pipe\n");
			if (fd_in != -1)
				close(fd_in);
			return (1);
		}
		if (fork_and_execute(cmd, mini, fd_in, pipe_fd[1]) == 1)
			return (1);
		if (fd_in != -1)
			close(fd_in);
		close(pipe_fd[1]);
	}
	else
	{
		if (fork_and_execute(cmd, mini, fd_in, STDOUT_FILENO) == 1)
			return (1);
		if (fd_in != -1)
			close(fd_in);
	}
	return (0);
}

void	handle_multiply_cmds(t_command *cmd, t_mini *mini)
{
	t_command	*current;
	int			fd_in;
	int			pipe_fd[2];

	fd_in = -1;
	current = cmd;
	while (current)
	{
		mini->last_exit_status = execute_pipeline_cmd(current, \
													mini, \
													fd_in, \
													pipe_fd);
		if (current->next)
			fd_in = pipe_fd[0];
		current = current->next;
	}
	wait_exit_statuses(mini);
}
