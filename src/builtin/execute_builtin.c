/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:08:08 by devriez           #+#    #+#             */
/*   Updated: 2025/10/31 13:48:34 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_command *cmd, t_env *local_env)
{
	int	exit_status;

	exit_status = 1;
	if (ft_strcmp(cmd->name, "echo") == 0)
		exit_status = echo_builtin(cmd);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		exit_status = cd_builtin(cmd, local_env);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		exit_status = pwd_builtin(local_env);
	else if (ft_strcmp(cmd->name, "export") == 0)
		exit_status = export_builtin(cmd, local_env);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		exit_status = unset_builtin(cmd, &local_env);
	else if (ft_strcmp(cmd->name, "env") == 0)
		exit_status = env_builtin(cmd, local_env);
	else if (ft_strcmp(cmd->name, "exit") == 0)
		exit_status = exit_builtin(cmd, local_env);
	return (exit_status);
}

bool	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 
		|| ft_strcmp(cmd, "cd") == 0 
		|| ft_strcmp(cmd, "pwd") == 0 
		|| ft_strcmp(cmd, "export") == 0 
		|| ft_strcmp(cmd, "unset") == 0 
		|| ft_strcmp(cmd, "env") == 0 
		|| ft_strcmp(cmd, "exit") == 0)
		return (true);
	return (false);
}
