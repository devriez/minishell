/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:20:57 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/15 20:32:11 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void child_setup(int fd_in, int fd_out)
{
	setpgid(0, 0);  // Put child in its own process group
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static int	execute_external(t_command *cmd, t_env *local_env)
{
	char	*cmd_path;
	char	**cmd_with_args;
	char	**envp;

	cmd_path = get_cmd_path(cmd->name, local_env);
	if (!cmd_path)
		return (printf("Command '%s' is not executable\n", cmd->name), 127);
	cmd_with_args = cmd->args;
	envp = env_to_char_array(local_env);
	if (!cmd_with_args || !envp)
	{
		free(cmd_path);
		free_arr(envp);
		return (printf("Error with memore allocation\n"), 1);
	}
	if (execve(cmd_path, cmd_with_args, envp) == -1)
	{
		free(cmd_path);
		free_arr(envp);
		return (printf("Error with execve\n"), 126);
	}
	return (0);
}

int	child_process(t_command *cmd, t_env *local_env)
{
	int		exit_status;

	if (cmd->redirections)
	{
		if (handle_redirect(cmd->redirections) == 1)
			exit(1);
	}
	if (is_builtin(cmd->name))
		exit_status = execute_builtin(cmd, local_env);
	else
		exit_status = execute_external(cmd, local_env);
	return (exit_status);
}

