/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:08:08 by devriez           #+#    #+#             */
/*   Updated: 2025/09/03 17:34:02 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_internal(t_command *cmd, t_env *lockal_env)
{
	if (ft_strcmp(cmd->name, "echo") == 0)
		echo_builtin(cmd);
	if (ft_strcmp(cmd->name, "cd") == 0)
		cd_builtin(cmd, lockal_env);
	if (ft_strcmp(cmd->name, "pwd") == 0)
		pwd_builtin(lockal_env);
	if (ft_strcmp(cmd->name, "export"))
		export_builtin(cmd, lockal_env);
	return (0);
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