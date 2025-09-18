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
#include "heredoc_handler.h"
#include <readline/readline.h>
#include "../libft/libft.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int	create_tempfile(void)
{
	char	*dir;
	char	templ[PATH_MAX];
	int		fd;

	dir = getenv("TMPDIR");
	if (!dir || !*dir)
		dir = "/tmp";
	ft_strlcpy(templ, dir, PATH_MAX);
	ft_strlcat(templ, "/minishell_tmp_XXXXXX", PATH_MAX);
	fd = mkstemp(templ);
	if (fd < 0)
		return (-1);
	unlink(templ);
	return (fd);
}

static int	write_to_tempfile(int fd, char *buffer)
{
	ssize_t	bytes_written;
	size_t	len;

	if (!buffer)
		return (1);
	len = ft_strlen(buffer);
	if (len == 0)
		return (1);
	bytes_written = write(fd, buffer, len);
	if (bytes_written < 0 || (size_t)bytes_written != len)
		return (0);
	return (1);
}

static char	*expand_heredoc(char *buffer, int type)
{
	if (type == 1)
		return (ft_strdup(buffer));
	return (ft_strdup(buffer));
}

static void	no_delim_warning(void)
{
	write(STDERR_FILENO, "minishell: warning: here-document delimited by end-of-file\n", 59);
}

static t_redirect	prepare_heredoc_redir(int fd)
{
	t_redirect	redirect;

	redirect.type = HEREDOC;
	redirect.file = NULL;
	redirect.heredoc_fd = fd;
	redirect.next = NULL;
	return (redirect);
}

static char	*add_line_to_content(char *content, char *line)
{
	char	*temp;
	char	*with_newline;

	temp = ft_strjoin(content, line);
	if (!temp)
		return (NULL);
	with_newline = ft_strjoin(temp, "\n");
	free(temp);
	return (with_newline);
}

static int	is_delimiter_reached(char *delim, char *line)
{
	if (!line || strcmp(delim, line) == 0)
	{
		if (line)
			free(line);
		else
			no_delim_warning();
		return (1);
	}
	return (0);
}

static char	*process_heredoc_line(char *content, char *line)
{
	char	*new_content;

	new_content = add_line_to_content(content, line);
	free(content);
	free(line);
	return (new_content);
}

static char	*read_heredoc_lines(char *delim)
{
	char	*content;
	char	*line;
	char	*new_content;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (is_delimiter_reached(delim, line))
			break ;
		new_content = process_heredoc_line(content, line);
		if (!new_content)
			return (NULL);
		content = new_content;
	}
	return (content);
}

static int	create_heredoc_file(char *content)
{
	int	fd;

	fd = create_tempfile();
	if (fd < 0)
		return (-1);
	if (!write_to_tempfile(fd, content))
	{
		close(fd);
		return (-1);
	}
	return (fd);
}

t_redirect	read_heredoc(char *delim, int type)
{
	char	*content;
	char	*expanded;
	int		fd;

	content = read_heredoc_lines(delim);
	if (!content)
		return (prepare_heredoc_redir(-1));
	expanded = expand_heredoc(content, type);
	free(content);
	if (!expanded)
		return (prepare_heredoc_redir(-1));
	fd = create_heredoc_file(expanded);
	free(expanded);
	if (fd < 0)
		return (prepare_heredoc_redir(-1));
	return (prepare_heredoc_redir(fd));
}

