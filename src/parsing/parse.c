/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 10:46:56 by johartma          #+#    #+#             */
/*   Updated: 2025/09/02 15:13:46 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/readline.h>
#include "../libft/libft.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

int	create_tempfile(void)
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
			break ;
		ft_strjoin(read, buffer);
		free(buffer);
	}
	// 2. Create Tempfile
	fd = create_tempfile();
	
	
}

