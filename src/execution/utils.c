/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:38:48 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/24 20:39:36 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	johannes_func(char	*line)
{
	t_command	res;

	return (res);
}

void	handle_signal(int signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	is_internal(char *cmd)
{
	if (cmd == "echo" || cmd == "cd" || cmd == "pwd" || cmd == "export" || \
		cmd == "unset" || cmd == "env" || cmd == "exit")
		return (1);
	return (0);
}

int	execute_internal(t_command *cmd, char **envp)
{
	return (0);
}

void	child_procces(t_command *cmd, char **envv, int *pipe_fd)
{

}