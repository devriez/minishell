/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:46:13 by devriez           #+#    #+#             */
/*   Updated: 2025/10/30 17:50:08 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fork_and_execute(t_command *cmd, \
							t_mini *mini, \
							int fd_in, \
							int fd_out)
{
	pid_t	pid;
	int		exit_status;

	pid = fork();
	if (pid == -1)
	{
		printf("Error with forking\n");
		return (1);
	}
	if (pid == 0)
	{
		exit_status = child_setup(cmd, fd_in, fd_out);
		if (exit_status == 0)
			exit_status = child_process(cmd, mini->env);
		return (exit_status);
	}
	setup_signals(ignore_sigint_print_newline);
	mini->pids[mini->pid_count] = pid;
	mini->pid_count++;
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
		return (execute_helper(cmd, mini, fd_in, pipe_fd));
	}
	else
	{
		if (fork_and_execute(cmd, mini, fd_in, STDOUT_FILENO) == 1)
		{
			if (fd_in != -1)
				close(fd_in);
			return (1);
		}
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
