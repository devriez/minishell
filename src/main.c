/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:47:11 by devriez           #+#    #+#             */
/*   Updated: 2025/10/31 12:55:17 by amoiseik         ###   ########.fr       */
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
	if (mini->last_exit_status == 131)
	{
		write(1, "Quit (core dumped)\n", 19);
	}
}

// static char	*read_input(t_mini *mini)
// {
// 	char	*line;

// 	line = readline("minishell> ");
// 	if (g_last_signal == SIGINT)
// 	{
// 		mini->last_exit_status = 130;
// 		g_last_signal = 0;
// 	}
// 	if (!line)
// 	{
// 		printf("exit\n");
// 		return (NULL);
// 	}
// 	add_history(line);
// 	return (line);
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	t_command	*cmd;
// 	t_mini		mini;
// 	char		*line;

// 	(void)argc;
// 	(void)argv;
// 	mini.last_exit_status = 0;
// 	setup_signals(set_sigint);
// 	mini.env = env_to_list(envp);
// 	mini.shell_name = argv[0];
// 	update_shlvl(mini.env);
// 	while (1)
// 	{
// 		line = read_input(&mini);
// 		if (!line)
// 			break ;
// 		if (ft_strcmp(line, "") == 0)
// 			continue ;
// 		cmd = parse_command_line(line, mini);
// 		free(line);
// 		execute_cmd(cmd, &mini);
// 		free_command(cmd);
// 	}
// 	free_env_list(mini.env);
// 	return (0);
// }

//!!!!!!!!!!!! delet next function and uncomment previous two !!!!!!!!!!!!!!!!1
int	main(int argc, char **argv, char **envp)
{
	t_command	*cmd;
	t_mini		mini;
	char		*line = "cat < /tmp/a_very_long_file_name_that_might_cause_issues.txt";

	(void)argc;
	(void)argv;
	mini.last_exit_status = 0;
	setup_signals(set_sigint);
	mini.env = env_to_list(envp);
	mini.shell_name = argv[0];
	update_shlvl(mini.env);
	// while (1)
	// {
	//line = read_input(&mini);
	// if (!line)
	// 	break ;
	// if (ft_strcmp(line, "") == 0)
	// 	continue ;
	cmd = parse_command_line(line, mini);
	//free(line);
	execute_cmd(cmd, &mini);
	free_command(cmd);
	// }
	free_env_list(mini.env);
	return (0);
}