/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_internal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:08:08 by devriez           #+#    #+#             */
/*   Updated: 2025/08/28 19:10:42 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_internal(t_command *cmd, t_env *lockal_envp)
{
	char *cmd_name;

	cmd_name = cmd->args[0];
	if (ft_strcmp(cmd_name,"echo") == 0)
		echo_buildtin(cmd);
	if (ft_strcmp(cmd_name,"cd") == 0)
		cd_buildtin(cmd);
	return (0);
}