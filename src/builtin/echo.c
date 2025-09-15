/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 19:08:36 by devriez           #+#    #+#             */
/*   Updated: 2025/09/11 20:53:12 by devriez          ###   ########.fr       */
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


static void	print_arg(t_env *env, char *arg)
{
	char	*val;

	if (arg[0] == '$')
	{
		val = get_env_var_val(env, arg + 1);
		if (val)
			printf("%s", val);
	}
	else
		printf("%s", arg);
}

int	echo_builtin(t_command *cmd, t_env *env)
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
		print_arg(env, cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}

