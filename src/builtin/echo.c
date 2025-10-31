/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 19:08:36 by devriez           #+#    #+#             */
/*   Updated: 2025/10/30 17:40:19 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_n_flag(char *flag)
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

static void	print_arg(char *arg)
{
	printf("%s", arg);
}

int	echo_builtin(t_command *cmd)
{
	int		i;
	bool	n_flag;

	i = 0;
	n_flag = false;
	if (!cmd->args || !cmd->args[0])
	{
		printf("\n");
		return (0);
	}
	while (cmd->args[i] && is_n_flag(cmd->args[i]))
	{
		n_flag = true;
		i++;
	}
	while (cmd->args[i])
	{
		print_arg(cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}
