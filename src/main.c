/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:47:11 by devriez           #+#    #+#             */
/*   Updated: 2025/09/15 19:35:17 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_exit_status = 0;

void	execute_cmd(t_command *cmd, t_env *local_env)
{
	if (!cmd)
		return ;
	if (cmd->next)
		handle_multiply_cmds(cmd, local_env);
	else
		handle_single_cmd(cmd, local_env);
}

static int	g_should_exit = 0;

t_command	*read_and_parse(void)
{
	char		*line;
	t_command	*cmd;

	line = readline("minishell> ");
	if (!line)
	{
		printf("exit\n");
		g_should_exit = 1;
		return (NULL);
	}
	if (line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	cmd = parse_command_line(line);
	free(line);
	return (cmd);
}

int	main(int argc, char **argv, char **envp)
{
	t_env		*local_env;
	t_command	*cmd;

	(void)argc;
	(void)argv;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	local_env = env_to_list(envp);
	while (1)
	{
		cmd = read_and_parse();
		if (g_should_exit)
			break ;
		if (!cmd)
			continue ;
		execute_cmd(cmd, local_env);
		free_command(cmd);
	}
	free_env_list(local_env);
	return (0);
}