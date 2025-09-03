/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:04:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/03 15:08:03 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_lockal_env(t_env *lockal_env)
{
	sort_env(lockal_env);
	while (lockal_env)
	{
		printf("%s=%s\n", lockal_env->name, lockal_env->value);
		lockal_env = lockal_env->next;
	}
}

int	export_builtin(t_command *cmd, t_env *lockal_env)
{
	int	status;
	int	i;

	if (!cmd->args)
	{
		print_lockal_env(lockal_env);
		return (0);
	}
	else
	{
		status = 0;
		i = 0;
		while (cmd->args[i])
		{
			if (set_envv_from_str(&lockal_env, cmd->args[i]))
				status = 1;
			i++;
		}
	}
	return (status);
}
