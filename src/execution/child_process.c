/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:20:57 by amoiseik          #+#    #+#             */
/*   Updated: 2025/10/30 17:58:53 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "errno.h"

int	child_setup(t_command *cmd, int fd_in, int fd_out)
{
	setup_signals_to_default();
	if (fd_in >= 0 && fd_in != STDIN_FILENO)
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
			return (perror("dup2 stdin"), 1);
		close(fd_in);
	}
	if (fd_out >= 0 && fd_out != STDOUT_FILENO)
	{
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2 stdout"), 1);
		close(fd_out);
	}
	if (cmd->redirections)
	{
		if (handle_redirect(cmd->redirections) == 1)
			return (1);
	}
	close_all_other_pipes(fd_in, fd_out);
	return (0);
}

static int	execve_fail_exit(char *cmd_name, char **envp)
{
	int			e;
	char *const	script_argv[] = {"/bin/sh", (char *)cmd_name, NULL};
	int			exit_code;
	const char	*error_msg;

	e = errno;
	exit_code = 126;
	if (e == ENOEXEC) 
	{
		execve("/bin/sh", script_argv, envp); 
		e = errno; 
	}
	if (e == ENOENT) 
	{
		error_msg = "No such file or directory";
		exit_code = 127;
	}
	else if (e == EACCES)
		error_msg = "Permission denied";
	else if (e == EISDIR)
		error_msg = "is a directory";
	else
		error_msg = strerror(e);
	put_error(cmd_name, error_msg);
	return (free_arr(envp), exit_code);
}

static int	execute_external(t_command *cmd, t_env *local_env)
{
	char	*cmd_path;
	char	**cmd_with_args;
	char	**envp;

	cmd_path = get_cmd_path(cmd->name, local_env);
	if (!cmd_path)
		return (cmd_error_not_found(cmd->name));
	if (check_is_directory(cmd_path, cmd->name) == 126)
		return (free(cmd_path), 126);
	cmd_with_args = join_str_with_arr(cmd->name, cmd->args);
	envp = env_to_char_array(local_env);
	if (!cmd_with_args || !envp)
	{
		free(cmd_path);
		free_arr(cmd_with_args);
		free_arr(envp);
		return (printf("Error with memory allocation\n"), 1);
	}
	if (execve(cmd_path, cmd_with_args, envp) == -1)
	{
		free(cmd_path);
		free_arr(cmd_with_args);
		return (execve_fail_exit(cmd->name, envp));
	}
	return (0);
}

int	child_process(t_command *cmd, t_env *local_env)
{
	int		exit_status;

	if (is_builtin(cmd->name))
		exit_status = execute_builtin(cmd, local_env);
	else
		exit_status = execute_external(cmd, local_env);
	return (exit_status);
}
