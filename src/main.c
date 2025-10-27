/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:47:11 by devriez           #+#    #+#             */
/*   Updated: 2025/10/27 17:52:32 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_last_signal = 0;

static void	execute_cmd(t_command *cmd, t_mini *mini)
{
	if (!cmd)
		return ;
	mini->pid_count = 0;
	if (cmd->next)
		handle_multiply_cmds(cmd, mini);
	else
		handle_single_cmd(cmd, mini);
}

static char	*read_input(t_mini *mini)
{
	char	*line;

	line = readline("minishell> ");
	if (g_last_signal == SIGINT)
	{
		mini->last_exit_status = 130;
		g_last_signal = 0;
	}
	if (!line)
	{
		printf("exit\n");
		return (NULL);
	}
	return (line);
}

int	main(int argc, char **argv, char **envp)
{
	t_command	*cmd;
	t_mini		mini;
	char		*line;

	(void)argc;
	(void)argv;
	setup_signals(set_sigint);
	mini.env = env_to_list(envp);
	mini.shell_name = argv[0]; //add
	update_shlvl(mini.env); //add
	while (1)
	{
		line = read_input(&mini);
		if (!line)
			break ;
		if (ft_strcmp(line, "") == 0)
			continue ;
		cmd = johannes_func(line);
		free(line);
		execute_cmd(cmd, &mini);
		free_command(cmd);
	}
	free_env_list(mini.env);
	return (0);
}
