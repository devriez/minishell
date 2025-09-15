/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:45:07 by devriez           #+#    #+#             */
/*   Updated: 2025/09/11 13:17:43 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_builtin(t_command *cmd, t_env **local_env)
{
	char	*var_name;
	int		exit_status;
	int		i;

	if (!cmd->args || !cmd->args[0])
		return (0);
	exit_status = 0;
	i = 0;
	while (cmd->args[i])
	{
		var_name = cmd->args[i];
		if (is_correct_varname(var_name))
			remove_envv(local_env, var_name);
		else
			exit_status = 1;
		i ++;
	}
	return (exit_status);
}
