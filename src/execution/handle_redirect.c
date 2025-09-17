/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:52:58 by devriez           #+#    #+#             */
/*   Updated: 2025/09/12 18:41:05 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_heredoc(t_redirect *redir)
{
	int	file_d_in;

	if (redir->type == INPUT)
		file_d_in = open(redir->file, O_RDONLY, 0644);
	else if (redir->type == HEREDOC)
		file_d_in = redir->heredoc_fd;
	else
		return (1);
	if (file_d_in == -1)
		return (perror("open"), 1);
	if (dup2(file_d_in, STDIN_FILENO) == -1)
	{
		perror("dup2");
		if (redir->type == INPUT)
			close(file_d_in);
		return (1);
	}
	if (redir->type == INPUT)
		close(file_d_in);
	return (0);
}

static int	handle_output_append(t_redirect *redir)
{
	int	file_d_out;

	if (redir->type == OUTPUT)
		file_d_out = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (redir->type == APPEND)
		file_d_out = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		return (1);
	if (file_d_out == -1)
		return (perror("open"), 1);
	if (dup2(file_d_out, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(file_d_out);
		return (1);
	}
	close(file_d_out);
	return (0);
}

int	handle_redirect(t_redirect *redirections)
{
	t_redirect	*redir;
	int			status;

	redir = redirections;
	while (redir)
	{
		if (redir->type == INPUT || redir->type == HEREDOC)
			status = handle_input_heredoc(redir);
		else if (redir->type == OUTPUT || redir->type == APPEND)
			status = handle_output_append(redir);
		if (status == 1)
			return (1);
		redir = redir->next;
	}
	return (0);
}

int	redirect_in_parent(t_redirect *redir, int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
		return (perror("dup"), 1);
	if (handle_redirect(redir) == 1)
	{
		dup2(*saved_stdin, STDIN_FILENO);
		dup2(*saved_stdout, STDOUT_FILENO);
		close(*saved_stdin);
		close(*saved_stdout);
		return (1);
	}
	return (0);
}

void	restore_parent_io(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
