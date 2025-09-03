/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:08:08 by devriez           #+#    #+#             */
/*   Updated: 2025/09/03 18:27:29 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_command *cmd, t_env *lockal_env)
{
	int	exit_status;

	exit_status = 1;
	if (ft_strcmp(cmd->name, "echo") == 0)
		exit_status = echo_builtin(cmd);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		exit_status = cd_builtin(cmd, lockal_env);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		exit_status = pwd_builtin(lockal_env);
	else if (ft_strcmp(cmd->name, "export"))
		exit_status = export_builtin(cmd, lockal_env);
	return (exit_status);
}

bool	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || \
		ft_strcmp(cmd, "cd") == 0 || \
		ft_strcmp(cmd, "pwd") == 0 || \
		ft_strcmp(cmd, "export") == 0 || \
		ft_strcmp(cmd, "unset") == 0 || \
		ft_strcmp(cmd, "env") == 0 || \
		ft_strcmp(cmd, "exit") == 0)
		return (true);
	return (false);
}
