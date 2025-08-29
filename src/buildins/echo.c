/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 19:08:36 by devriez           #+#    #+#             */
/*   Updated: 2025/08/27 23:18:14 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_n_flag(t_command *cmd)
{
	char *flags;

	flags = cmd->args[1];
	if(!flags || *flags != '-')
		return (false);
	flags ++;
	while (*flags != '\0' && *flags == 'n')
		flags ++;
	if (*flags == '\0')
		return (true);
	else
		return (false);
}

int	echo_buildtin(t_command *cmd)
{
	char	**text;

	text = cmd->args;
	text ++;
	if (has_n_flag(cmd))
		text ++;
	while (*text)
	{
		printf("%s", *text);
		if (text ++)
			printf(" ");
	}
	if (has_n_flag(cmd))
		printf("\n");
	return (0);
}
