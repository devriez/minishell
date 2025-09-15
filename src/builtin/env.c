/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 17:04:54 by devriez           #+#    #+#             */
/*   Updated: 2025/09/15 19:56:59 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_builtin(t_command *cmd, t_env *local_env)
{
	t_env	*env_node;

	if (cmd->args && cmd->args[1])
	{
		printf("env: %s: No such file or directory\n", cmd->args[1]);
		return (127);
	}
	env_node = local_env;
	while (env_node)
	{
		if (env_node->value)
			printf("%s=%s\n", env_node->name, env_node->value);
		env_node = env_node->next;
	}
	return (0);
}