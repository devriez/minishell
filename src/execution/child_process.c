/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:20:57 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/03 20:00:17 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_external(t_command *cmd, t_env *lockal_env)
{
	char	*cmd_path;

	cmd_path = get_cmd_path(cmd->name, lockal_env);
	if (!cmd_path)
	{
		printf("Command '%s' is not executable", cmd->name);
		return (1);
	}
	if (execve(cmd_path, cmd, envv) == -1) //thinking here
	{
		free(cmd_path);
		printf("Error with execve");
		return (1);
	}
	return (0);
}

int	child_process(t_command *cmd, t_env *lockal_env)
{
	int		exit_status;

	if (is_builtin(cmd->name))
		exit_status = execute_builtin(cmd, lockal_env);
	else
		exit_status = execute_external(cmd, lockal_env);
	return (exit_status);
}

