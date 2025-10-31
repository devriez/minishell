/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 13:26:26 by devriez           #+#    #+#             */
/*   Updated: 2025/10/31 13:49:21 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long long	ft_atoi_long(const char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	while (*str && ft_isspace(*str))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str && ft_isdigit(*str))
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

static bool	is_numeric(char *str)
{
	if (!str || !*str)
		return (false);
	while (*str && ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

int	exit_builtin(t_command *cmd, t_env *local_env)
{
	long long int	exit_status;

	printf("exit\n");
	if (!cmd->args || !cmd->args[0])
		exit_status = 0;
	else if (!is_numeric(cmd->args[0]))
	{
		printf("minishell: exit: %s: numeric argument required\n", cmd->args[0]);
		exit_status = 255;
	}
	else if (cmd->args[1])
	{
		printf("minishell: exit: too many arguments\n");
		exit_status = 1;
	}
	else
		exit_status = (unsigned char)ft_atoi_long(cmd->args[0]);
	free_env_list(local_env);
	free_command(cmd);
	exit(exit_status);
}
