/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heredoc_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/09/12 13:39:21 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include "../heredoc_handler.h"

/* Test helper functions */

static void	cleanup_redirect(t_redirect *redir)
{
	if (redir && redir->heredoc_fd >= 0)
		close(redir->heredoc_fd);
	if (redir && redir->file)
		free(redir->file);
}

static char	*read_fd_content(int fd)
{
	char	buffer[1024];
	char	*content;
	char	*temp;
	ssize_t	bytes_read;
	
	content = ft_strdup("");
	if (!content)
		return (NULL);
	lseek(fd, 0, SEEK_SET);
	while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(content, buffer);
		free(content);
		if (!temp)
			return (NULL);
		content = temp;
	}
	return (content);
}

/* Mock readline function for testing */
static char	*g_test_input[100];
static int	g_input_index = 0;

static void	setup_mock_input(char **input)
{
	int	i;

	g_input_index = 0;
	i = 0;
	while (i < 100 && input[i])
	{
		g_test_input[i] = input[i];
		i++;
	}
	g_test_input[i] = NULL;
}

/* Note: This is a simplified mock for testing.
 * In reality, you'd need to link against readline 
 * or provide a more sophisticated mock */
char	*readline(const char *prompt)
{
	char	*line;

	(void)prompt;
	if (g_test_input[g_input_index] == NULL)
		return (NULL);
	line = ft_strdup(g_test_input[g_input_index]);
	g_input_index++;
	return (line);
}

/* Test cases */

static void	test_basic_heredoc(void)
{
	t_redirect	result;
	char		*content;
	char		*input[] = {"line1", "line2", "EOF", NULL};

	printf("Testing basic heredoc...\n");
	setup_mock_input(input);
	result = read_heredoc("EOF", 0);
	
	assert(result.type == HEREDOC);
	assert(result.heredoc_fd >= 0);
	assert(result.file == NULL);
	
	content = read_fd_content(result.heredoc_fd);
	assert(content != NULL);
	assert(strcmp(content, "line1\nline2\n") == 0);
	
	free(content);
	cleanup_redirect(&result);
	printf("✓ Basic heredoc test passed\n");
}

static void	test_empty_heredoc(void)
{
	t_redirect	result;
	char		*content;
	char		*input[] = {"EOF", NULL};

	printf("Testing empty heredoc...\n");
	setup_mock_input(input);
	result = read_heredoc("EOF", 0);
	
	assert(result.type == HEREDOC);
	assert(result.heredoc_fd >= 0);
	
	content = read_fd_content(result.heredoc_fd);
	assert(content != NULL);
	assert(strcmp(content, "") == 0);
	
	free(content);
	cleanup_redirect(&result);
	printf("✓ Empty heredoc test passed\n");
}

static void	test_heredoc_with_quotes(void)
{
	t_redirect	result;
	char		*content;
	char		*input[] = {"line with \"quotes\"", "and 'single quotes'", "END", NULL};

	printf("Testing heredoc with quotes...\n");
	setup_mock_input(input);
	result = read_heredoc("END", 1);
	
	assert(result.type == HEREDOC);
	assert(result.heredoc_fd >= 0);
	
	content = read_fd_content(result.heredoc_fd);
	assert(content != NULL);
	assert(strcmp(content, "line with \"quotes\"\nand 'single quotes'\n") == 0);
	
	free(content);
	cleanup_redirect(&result);
	printf("✓ Heredoc with quotes test passed\n");
}

static void	test_heredoc_eof_warning(void)
{
	t_redirect	result;
	char		*input[] = {NULL};

	printf("Testing heredoc EOF warning...\n");
	setup_mock_input(input);
	result = read_heredoc("DELIMITER", 0);
	
	/* Should still create a valid redirect even with EOF */
	assert(result.type == HEREDOC);
	assert(result.heredoc_fd >= 0);
	
	cleanup_redirect(&result);
	printf("✓ Heredoc EOF warning test passed\n");
}

static void	test_single_line_heredoc(void)
{
	t_redirect	result;
	char		*content;
	char		*input[] = {"single line", "STOP", NULL};

	printf("Testing single line heredoc...\n");
	setup_mock_input(input);
	result = read_heredoc("STOP", 0);
	
	assert(result.type == HEREDOC);
	assert(result.heredoc_fd >= 0);
	
	content = read_fd_content(result.heredoc_fd);
	assert(content != NULL);
	assert(strcmp(content, "single line\n") == 0);
	
	free(content);
	cleanup_redirect(&result);
	printf("✓ Single line heredoc test passed\n");
}

int	main(void)
{
	printf("=== Heredoc Handler Test Suite ===\n\n");
	
	test_basic_heredoc();
	test_empty_heredoc();
	test_heredoc_with_quotes();
	test_heredoc_eof_warning();
	test_single_line_heredoc();
	
	printf("\n=== All tests passed! ===\n");
	return (0);
}


