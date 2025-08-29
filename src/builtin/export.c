/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:04:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/29 20:47:41 by amoiseik         ###   ########.fr       */
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
	return (0);
}

int	echo_builtin(t_command *cmd, t_env *lockal_env)
{
	char	**cmd_args;

	cmd_args = cmd->args ++;
	if (!*cmd_args)
		print_lockal_env(lockal_env);
	else
		while (cmd_args)
		{
			if()
		}
}

