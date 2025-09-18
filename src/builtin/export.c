/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:04:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/18 09:32:50 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_local_env(t_env *local_env)
{
	sort_env(local_env);
	while (local_env)
	{
		printf("declare -x %s=%s\n", local_env->name, local_env->value);
		local_env = local_env->next;
	}
}

int	export_builtin(t_command *cmd, t_env *local_env)
{
	int	status;
	int	i;

	status = 0;
	if (!cmd->args || !cmd->args[0])
		print_local_env(local_env);
	else
	{
		i = 0;  // Start from first argument
		while (cmd->args[i])
		{
			if (set_envv_from_str(&local_env, cmd->args[i]))
				status = 1;
			i++;
		}
	}
	return (status);
}
