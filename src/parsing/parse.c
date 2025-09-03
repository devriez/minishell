/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 10:46:56 by johartma          #+#    #+#             */
/*   Updated: 2025/09/03 20:12:51 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/readline.h>
#include "../libft/libft.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

static int	create_tempfile(void)
{
	char	*dir;
	char	templ[PATH_MAX];
	int		fd;

	dir = getenv("TMPDIR");
	if (!dir | !*dir)
		dir = "/tmp";
	ft_strlcat(templ, dir, PATH_MAX);
	free(dir);
	ft_strlcat(templ, "/mishell_tmp_XXXXXX", PATH_MAX);
	fd = mkstmp(templ);
	if (fd < 0)
		return (-1);
	unlink(templ);
	return (fd);
}

static int	write_to_tempfile(int fd, char *buffer);

static char	*expand_heredoc(char *buffer, char *delim);

static void no_delim_warning(void);

static t_redirect	prepare_heredoc_redir(int fd)
{
	t_redirect	redirect;

	redirect.type = HEREDOC;
	redirect.file = NULL;
	redirect.heredoc_fd = fd;
	redirect.next = NULL;
	return (redirect);
}

t_redirect	read_heredoc(char *delim, int type)
{
	int		fd;
	char	*tmpfilepath;
	char	*buffer;
	char	*read;
	
	// 1. Read lines to read
	read = 0;

	while (1)
	{
		buffer = readline(NULL);
		if (!buffer || !strcmp(delim, buffer))
		{
			if (buffer)
				free(buffer);
			else
				no_delim_warning();
			break ;
		}
		ft_strjoin(read, buffer);
		free(buffer);
	}
	buffer = expand_heredoc(buffer, type);
	fd = create_tempfile();
	if (!fd)
		handle_tmp_creation_err_here();
	if (!write_to_tempfile(fd, buffer))
		handle_write_error_here();
	return (prepare_heredoc_redir(fd));
}

