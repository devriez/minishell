/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 10:46:56 by johartma          #+#    #+#             */
/*   Updated: 2025/09/03 20:12:51 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../../includes/minishell.h"

#define HEREDOC 3

static void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

static t_redirect	prepare_heredoc_redir(int fd)
{
	t_redirect	r;

	r.type = HEREDOC;
	r.file = NULL;
	r.heredoc_fd = fd;
	r.next = NULL;
	return (r);
}

static int	is_delimiter_reached(char *delim, char *line)
{
	if (!line || strcmp(delim, line) == 0)
	{
		if (line)
			free(line);
		else
			write(STDERR_FILENO,
				"minishell: warning: here-document delimited\
				 by end-of-file\n", 59);
		return (1);
	}
	return (0);
}

static void	child_heredoc(char *delim, int type, int pipefd[2])
{
	char				*line;
	struct sigaction	act;

	(void)type;
	close(pipefd[0]);
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = sigint_handler;
	sigaction(SIGINT, &act, NULL);
	while (1)
	{
		line = readline("> ");
		if (is_delimiter_reached(delim, line))
			break ;
		if (line && *line)
			add_history(line);
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	exit(0);
}

t_redirect	read_heredoc(char *delim, int type)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) < 0)
		return (prepare_heredoc_redir(-1));
	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (prepare_heredoc_redir(-1));
	}
	if (pid == 0)
		child_heredoc(delim, type, pipefd);
	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
	if (WIFSIGNALED(status))
	{
		close(pipefd[0]);
		return (prepare_heredoc_redir(-1));
	}
	return (prepare_heredoc_redir(pipefd[0]));
}
