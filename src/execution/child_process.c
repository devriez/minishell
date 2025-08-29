/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:20:57 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/29 16:24:48 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(char *cmd_from_input, char **envp)
{
	char	**cmd;
	char	*cmd_path;

	cmd = ft_split(cmd_from_input, ' ');
	cmd_path = get_cmd_path(cmd[0], envp);
	if (!cmd_path)
	{
		free_arr(cmd);
		log_error("command is not executable");
	}
	else if (execve(cmd_path, cmd, envp) == -1)
	{
		free(cmd_path);
		free_arr(cmd);
		sys_error("Error with execve");
	}
}

void	child_process(t_command *cmd, char **envp)
{
	char	cmd_name;

	cmd_name = cmd->args[0];

	if (is_builtin(cmd_name))
		g_last_exit_status = execute_internal(cmd, envp);
	else
		execute_external(cmd, envp);
}

