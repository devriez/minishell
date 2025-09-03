/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 19:08:36 by devriez           #+#    #+#             */
/*   Updated: 2025/09/03 17:30:03 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_n_flag(char *flag)
{
	if (!flag || *flag != '-')
		return (false);
	flag ++;
	if (*flag == '\0')
		return (false);
	while (*flag != '\0' && *flag == 'n')
		flag ++;
	if (*flag == '\0')
		return (true);
	else
		return (false);
}

bool	is_empty_flag(char *flag)
{
	if (!flag || *flag != '-')
		return (false);
	flag ++;
	if (*flag == '\0')
		return (true);
	return (false);
}

int	echo_builtin(t_command *cmd)
{
	int		i;
	bool	n_flag_found;

	i = 0;
	n_flag_found = false;
	if (!cmd->args)
		return (printf("\n"), 0);
	while (cmd->args[i] && is_n_flag(cmd->args[i]))
	{
		n_flag_found = true;
		i ++;
	}
	if (is_empty_flag(cmd->args[i]))
		i ++;
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i ++;
	}
	if (n_flag_found != true)
		printf("\n");
	return (0);
}
