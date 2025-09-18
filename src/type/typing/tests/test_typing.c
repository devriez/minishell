/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_typing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/09/15 19:16:51 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../typing.h"

/* Test helper functions */

static char	**create_test_tokens(const char *tokens[], size_t count)
{
	char	**result;
	size_t	i;

	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < count)
	{
		result[i] = strdup(tokens[i]);
		i++;
	}
	result[count] = NULL;
	return (result);
}

static void	free_test_tokens(char **tokens)
{
	size_t	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

static void	free_typed_tokens(t_tokens *tokens, size_t count)
{
	size_t	i;

	if (!tokens)
		return ;
	i = 0;
	while (i < count)
	{
		/* Note: tokens[i].text points to the original string, 
		   so we don't free it here */
		i++;
	}
	free(tokens);
}

/* Test cases */

static void	test_basic_word_classification(void)
{
	const char	*input[] = {"echo", "hello", "world"};
	char		**tokens;
	t_tokens	*typed_tokens;

	printf("Testing basic word classification...\n");
	tokens = create_test_tokens(input, 3);
	typed_tokens = get_type(tokens, &typed_tokens);
	
	assert(typed_tokens != NULL);
	assert(typed_tokens[0].type == W_WORD);
	assert(strcmp(typed_tokens[0].text, "echo") == 0);
	assert(typed_tokens[0].len == 4);
	
	assert(typed_tokens[1].type == W_WORD);
	assert(strcmp(typed_tokens[1].text, "hello") == 0);
	assert(typed_tokens[1].len == 5);
	
	assert(typed_tokens[2].type == W_WORD);
	assert(strcmp(typed_tokens[2].text, "world") == 0);
	assert(typed_tokens[2].len == 5);
	
	free_typed_tokens(typed_tokens, 3);
	free_test_tokens(tokens);
	printf("✓ Basic word classification test passed\n");
}

static void	test_pipe_classification(void)
{
	const char	*input[] = {"echo", "hello", "|", "grep", "h"};
	char		**tokens;
	t_tokens	*typed_tokens;

	printf("Testing pipe classification...\n");
	tokens = create_test_tokens(input, 5);
	typed_tokens = get_type(tokens, &typed_tokens);
	
	assert(typed_tokens != NULL);
	assert(typed_tokens[0].type == W_WORD);
	assert(typed_tokens[1].type == W_WORD);
	assert(typed_tokens[2].type == W_PIPE);
	assert(strcmp(typed_tokens[2].text, "|") == 0);
	assert(typed_tokens[3].type == W_WORD);
	assert(typed_tokens[4].type == W_WORD);
	
	free_typed_tokens(typed_tokens, 5);
	free_test_tokens(tokens);
	printf("✓ Pipe classification test passed\n");
}

static void	test_input_redirection(void)
{
	const char	*input[] = {"cat", "<", "file.txt"};
	char		**tokens;
	t_tokens	*typed_tokens;

	printf("Testing input redirection...\n");
	tokens = create_test_tokens(input, 3);
	typed_tokens = get_type(tokens, &typed_tokens);
	
	assert(typed_tokens != NULL);
	assert(typed_tokens[0].type == W_WORD);
	assert(typed_tokens[1].type == W_INPUT);
	assert(strcmp(typed_tokens[1].text, "<") == 0);
	assert(typed_tokens[2].type == W_WORD);
	
	free_typed_tokens(typed_tokens, 3);
	free_test_tokens(tokens);
	printf("✓ Input redirection test passed\n");
}

static void	test_output_redirection(void)
{
	const char	*input[] = {"echo", "hello", ">", "output.txt"};
	char		**tokens;
	t_tokens	*typed_tokens;

	printf("Testing output redirection...\n");
	tokens = create_test_tokens(input, 4);
	typed_tokens = get_type(tokens, &typed_tokens);
	
	assert(typed_tokens != NULL);
	assert(typed_tokens[0].type == W_WORD);
	assert(typed_tokens[1].type == W_WORD);
	assert(typed_tokens[2].type == W_OUTPUT);
	assert(strcmp(typed_tokens[2].text, ">") == 0);
	assert(typed_tokens[3].type == W_WORD);
	
	free_typed_tokens(typed_tokens, 4);
	free_test_tokens(tokens);
	printf("✓ Output redirection test passed\n");
}

static void	test_append_redirection(void)
{
	const char	*input[] = {"echo", "hello", ">>", "log.txt"};
	char		**tokens;
	t_tokens	*typed_tokens;

	printf("Testing append redirection...\n");
	tokens = create_test_tokens(input, 4);
	typed_tokens = get_type(tokens, &typed_tokens);
	
	assert(typed_tokens != NULL);
	assert(typed_tokens[0].type == W_WORD);
	assert(typed_tokens[1].type == W_WORD);
	assert(typed_tokens[2].type == W_APPEND);
	assert(strcmp(typed_tokens[2].text, ">>") == 0);
	assert(typed_tokens[3].type == W_WORD);
	
	free_typed_tokens(typed_tokens, 4);
	free_test_tokens(tokens);
	printf("✓ Append redirection test passed\n");
}

static void	test_heredoc_redirection(void)
{
	const char	*input[] = {"cat", "<<", "EOF"};
	char		**tokens;
	t_tokens	*typed_tokens;

	printf("Testing heredoc redirection...\n");
	tokens = create_test_tokens(input, 3);
	typed_tokens = get_type(tokens, &typed_tokens);
	
	assert(typed_tokens != NULL);
	assert(typed_tokens[0].type == W_WORD);
	assert(typed_tokens[1].type == W_HEREDOC);
	assert(strcmp(typed_tokens[1].text, "<<") == 0);
	assert(typed_tokens[2].type == W_WORD);
	assert(typed_tokens[2].meta.is_here_delim == 1);
	
	free_typed_tokens(typed_tokens, 3);
	free_test_tokens(tokens);
	printf("✓ Heredoc redirection test passed\n");
}

static void	test_complex_command(void)
{
	const char	*input[] = {"cat", "<", "input.txt", "|", "grep", "test", ">>", "result.txt"};
	char		**tokens;
	t_tokens	*typed_tokens;

	printf("Testing complex command with multiple redirections...\n");
	tokens = create_test_tokens(input, 8);
	typed_tokens = get_type(tokens, &typed_tokens);
	
	assert(typed_tokens != NULL);
	assert(typed_tokens[0].type == W_WORD);    // cat
	assert(typed_tokens[1].type == W_INPUT);   // <
	assert(typed_tokens[2].type == W_WORD);    // input.txt
	assert(typed_tokens[3].type == W_PIPE);    // |
	assert(typed_tokens[4].type == W_WORD);    // grep
	assert(typed_tokens[5].type == W_WORD);    // test
	assert(typed_tokens[6].type == W_APPEND);  // >>
	assert(typed_tokens[7].type == W_WORD);    // result.txt
	
	free_typed_tokens(typed_tokens, 8);
	free_test_tokens(tokens);
	printf("✓ Complex command test passed\n");
}

static void	test_empty_input(void)
{
	char		**tokens;
	t_tokens	*typed_tokens;

	printf("Testing empty input...\n");
	tokens = malloc(sizeof(char *));
	tokens[0] = NULL;
	typed_tokens = get_type(tokens, &typed_tokens);
	
	assert(typed_tokens != NULL);
	/* For empty input, we should get an empty array */
	
	free_typed_tokens(typed_tokens, 0);
	free(tokens);
	printf("✓ Empty input test passed\n");
}

static void	test_single_token(void)
{
	const char	*input[] = {"ls"};
	char		**tokens;
	t_tokens	*typed_tokens;

	printf("Testing single token...\n");
	tokens = create_test_tokens(input, 1);
	typed_tokens = get_type(tokens, &typed_tokens);
	
	assert(typed_tokens != NULL);
	assert(typed_tokens[0].type == W_WORD);
	assert(strcmp(typed_tokens[0].text, "ls") == 0);
	assert(typed_tokens[0].len == 2);
	assert(typed_tokens[0].meta.has_qoutes == 0);
	assert(typed_tokens[0].meta.is_here_delim == 0);
	
	free_typed_tokens(typed_tokens, 1);
	free_test_tokens(tokens);
	printf("✓ Single token test passed\n");
}

static void	test_multiple_heredoc(void)
{
	const char	*input[] = {"cmd", "<<", "EOF1", "arg", "<<", "EOF2"};
	char		**tokens;
	t_tokens	*typed_tokens;

	printf("Testing multiple heredocs...\n");
	tokens = create_test_tokens(input, 6);
	typed_tokens = get_type(tokens, &typed_tokens);
	
	assert(typed_tokens != NULL);
	assert(typed_tokens[0].type == W_WORD);     // cmd
	assert(typed_tokens[1].type == W_HEREDOC);  // <<
	assert(typed_tokens[2].type == W_WORD);     // EOF1
	assert(typed_tokens[2].meta.is_here_delim == 1);
	assert(typed_tokens[3].type == W_WORD);     // arg
	assert(typed_tokens[3].meta.is_here_delim == 0);
	assert(typed_tokens[4].type == W_HEREDOC);  // <<
	assert(typed_tokens[5].type == W_WORD);     // EOF2
	assert(typed_tokens[5].meta.is_here_delim == 1);
	
	free_typed_tokens(typed_tokens, 6);
	free_test_tokens(tokens);
	printf("✓ Multiple heredoc test passed\n");
}

int	main(void)
{
	printf("=== Typing Module Test Suite ===\n\n");
	
	test_basic_word_classification();
	test_pipe_classification();
	test_input_redirection();
	test_output_redirection();
	test_append_redirection();
	test_heredoc_redirection();
	test_complex_command();
	test_empty_input();
	test_single_token();
	test_multiple_heredoc();
	
	printf("\n=== All tests passed! ===\n");
	return (0);
}
