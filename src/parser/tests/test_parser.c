/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
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
#include "../parser.h"

/* Test helper functions */

static t_tokens	*create_test_tokens(const char *texts[], t_word_type types[], size_t count)
{
	t_tokens	*tokens;
	size_t		i;

	tokens = malloc(sizeof(t_tokens) * count);
	if (!tokens)
		return (NULL);
	i = 0;
	while (i < count)
	{
		tokens[i].text = strdup(texts[i]);
		tokens[i].type = types[i];
		tokens[i].len = strlen(texts[i]);
		tokens[i].mask = NULL;
		tokens[i].meta.has_qoutes = 0;
		tokens[i].meta.is_here_delim = 0;
		tokens[i].meta.delim_qouted = 0;
		i++;
	}
	return (tokens);
}

static void	free_test_tokens(t_tokens *tokens, size_t count)
{
	size_t	i;

	if (!tokens)
		return ;
	i = 0;
	while (i < count)
	{
		free(tokens[i].text);
		i++;
	}
	free(tokens);
}

static void	free_commands(t_command *cmd)
{
	t_command	*next;
	t_redirect	*redir;
	t_redirect	*next_redir;
	size_t		i;

	while (cmd)
	{
		next = cmd->next;
		free(cmd->name);
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		redir = cmd->redirections;
		while (redir)
		{
			next_redir = redir->next;
			free(redir->file);
			free(redir);
			redir = next_redir;
		}
		free(cmd);
		cmd = next;
	}
}

static t_env	*create_dummy_env(void)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	env->name = strdup("TEST");
	env->value = strdup("value");
	env->next = NULL;
	return (env);
}

static void	free_dummy_env(t_env *env)
{
	free(env->name);
	free(env->value);
	free(env);
}

/* Test cases */

static void	test_simple_command(void)
{
	const char		*texts[] = {"echo", "hello", "world"};
	t_word_type		types[] = {W_WORD, W_WORD, W_WORD};
	t_tokens		*tokens;
	t_command		*cmd;
	t_env			*env;

	printf("Testing simple command parsing...\n");
	tokens = create_test_tokens(texts, types, 3);
	env = create_dummy_env();
	
	cmd = parse_tokens_to_commands(tokens, 3, env);
	
	assert(cmd != NULL);
	assert(strcmp(cmd->name, "echo") == 0);
	assert(cmd->args != NULL);
	assert(strcmp(cmd->args[0], "echo") == 0);
	assert(strcmp(cmd->args[1], "hello") == 0);
	assert(strcmp(cmd->args[2], "world") == 0);
	assert(cmd->args[3] == NULL);
	assert(cmd->redirections == NULL);
	assert(cmd->next == NULL);
	
	free_commands(cmd);
	free_test_tokens(tokens, 3);
	free_dummy_env(env);
	printf("✓ Simple command test passed\n");
}

static void	test_command_with_output_redirect(void)
{
	const char		*texts[] = {"echo", "hello", ">", "output.txt"};
	t_word_type		types[] = {W_WORD, W_WORD, W_OUTPUT, W_WORD};
	t_tokens		*tokens;
	t_command		*cmd;
	t_env			*env;

	printf("Testing command with output redirection...\n");
	tokens = create_test_tokens(texts, types, 4);
	env = create_dummy_env();
	
	cmd = parse_tokens_to_commands(tokens, 4, env);
	
	assert(cmd != NULL);
	assert(strcmp(cmd->name, "echo") == 0);
	assert(cmd->args != NULL);
	assert(strcmp(cmd->args[0], "echo") == 0);
	assert(strcmp(cmd->args[1], "hello") == 0);
	assert(cmd->args[2] == NULL);
	assert(cmd->redirections != NULL);
	assert(cmd->redirections->type == OUTPUT);
	assert(strcmp(cmd->redirections->file, "output.txt") == 0);
	assert(cmd->redirections->next == NULL);
	
	free_commands(cmd);
	free_test_tokens(tokens, 4);
	free_dummy_env(env);
	printf("✓ Output redirection test passed\n");
}

static void	test_command_with_input_redirect(void)
{
	const char		*texts[] = {"cat", "<", "input.txt"};
	t_word_type		types[] = {W_WORD, W_INPUT, W_WORD};
	t_tokens		*tokens;
	t_command		*cmd;
	t_env			*env;

	printf("Testing command with input redirection...\n");
	tokens = create_test_tokens(texts, types, 3);
	env = create_dummy_env();
	
	cmd = parse_tokens_to_commands(tokens, 3, env);
	
	assert(cmd != NULL);
	assert(strcmp(cmd->name, "cat") == 0);
	assert(cmd->redirections != NULL);
	assert(cmd->redirections->type == INPUT);
	assert(strcmp(cmd->redirections->file, "input.txt") == 0);
	
	free_commands(cmd);
	free_test_tokens(tokens, 3);
	free_dummy_env(env);
	printf("✓ Input redirection test passed\n");
}

static void	test_pipe_commands(void)
{
	const char		*texts[] = {"echo", "hello", "|", "grep", "h"};
	t_word_type		types[] = {W_WORD, W_WORD, W_PIPE, W_WORD, W_WORD};
	t_tokens		*tokens;
	t_command		*cmd;
	t_env			*env;

	printf("Testing pipe commands...\n");
	tokens = create_test_tokens(texts, types, 5);
	env = create_dummy_env();
	
	cmd = parse_tokens_to_commands(tokens, 5, env);
	
	assert(cmd != NULL);
	assert(strcmp(cmd->name, "echo") == 0);
	assert(strcmp(cmd->args[0], "echo") == 0);
	assert(strcmp(cmd->args[1], "hello") == 0);
	assert(cmd->args[2] == NULL);
	
	assert(cmd->next != NULL);
	assert(strcmp(cmd->next->name, "grep") == 0);
	assert(strcmp(cmd->next->args[0], "grep") == 0);
	assert(strcmp(cmd->next->args[1], "h") == 0);
	assert(cmd->next->args[2] == NULL);
	assert(cmd->next->next == NULL);
	
	free_commands(cmd);
	free_test_tokens(tokens, 5);
	free_dummy_env(env);
	printf("✓ Pipe commands test passed\n");
}

static void	test_complex_pipeline(void)
{
	const char		*texts[] = {"cat", "<", "file.txt", "|", "grep", "test", "|", "wc", "-l"};
	t_word_type		types[] = {W_WORD, W_INPUT, W_WORD, W_PIPE, W_WORD, W_WORD, W_PIPE, W_WORD, W_WORD};
	t_tokens		*tokens;
	t_command		*cmd;
	t_env			*env;

	printf("Testing complex pipeline...\n");
	tokens = create_test_tokens(texts, types, 9);
	env = create_dummy_env();
	
	cmd = parse_tokens_to_commands(tokens, 9, env);
	
	assert(cmd != NULL);
	/* First command: cat < file.txt */
	assert(strcmp(cmd->name, "cat") == 0);
	assert(cmd->redirections != NULL);
	assert(cmd->redirections->type == INPUT);
	assert(strcmp(cmd->redirections->file, "file.txt") == 0);
	
	/* Second command: grep test */
	assert(cmd->next != NULL);
	assert(strcmp(cmd->next->name, "grep") == 0);
	assert(strcmp(cmd->next->args[1], "test") == 0);
	
	/* Third command: wc -l */
	assert(cmd->next->next != NULL);
	assert(strcmp(cmd->next->next->name, "wc") == 0);
	assert(strcmp(cmd->next->next->args[1], "-l") == 0);
	assert(cmd->next->next->next == NULL);
	
	free_commands(cmd);
	free_test_tokens(tokens, 9);
	free_dummy_env(env);
	printf("✓ Complex pipeline test passed\n");
}

static void	test_multiple_redirections(void)
{
	const char		*texts[] = {"cmd", "<", "in.txt", ">", "out.txt", ">>", "log.txt"};
	t_word_type		types[] = {W_WORD, W_INPUT, W_WORD, W_OUTPUT, W_WORD, W_APPEND, W_WORD};
	t_tokens		*tokens;
	t_command		*cmd;
	t_redirect		*redir;
	t_env			*env;

	printf("Testing multiple redirections...\n");
	tokens = create_test_tokens(texts, types, 7);
	env = create_dummy_env();
	
	cmd = parse_tokens_to_commands(tokens, 7, env);
	
	assert(cmd != NULL);
	assert(strcmp(cmd->name, "cmd") == 0);
	assert(cmd->redirections != NULL);
	
	/* Check first redirection: < in.txt */
	redir = cmd->redirections;
	assert(redir->type == INPUT);
	assert(strcmp(redir->file, "in.txt") == 0);
	
	/* Check second redirection: > out.txt */
	assert(redir->next != NULL);
	redir = redir->next;
	assert(redir->type == OUTPUT);
	assert(strcmp(redir->file, "out.txt") == 0);
	
	/* Check third redirection: >> log.txt */
	assert(redir->next != NULL);
	redir = redir->next;
	assert(redir->type == APPEND);
	assert(strcmp(redir->file, "log.txt") == 0);
	assert(redir->next == NULL);
	
	free_commands(cmd);
	free_test_tokens(tokens, 7);
	free_dummy_env(env);
	printf("✓ Multiple redirections test passed\n");
}

static void	test_heredoc_parsing(void)
{
	const char		*texts[] = {"cat", "<<", "EOF"};
	t_word_type		types[] = {W_WORD, W_HEREDOC, W_WORD};
	t_tokens		*tokens;
	t_command		*cmd;
	t_env			*env;

	printf("Testing heredoc parsing...\n");
	tokens = create_test_tokens(texts, types, 3);
	tokens[2].meta.is_here_delim = 1;  /* Mark as heredoc delimiter */
	env = create_dummy_env();
	
	cmd = parse_tokens_to_commands(tokens, 3, env);
	
	assert(cmd != NULL);
	assert(strcmp(cmd->name, "cat") == 0);
	assert(cmd->redirections != NULL);
	assert(cmd->redirections->type == HEREDOC);
	
	free_commands(cmd);
	free_test_tokens(tokens, 3);
	free_dummy_env(env);
	printf("✓ Heredoc parsing test passed\n");
}

static void	test_empty_command(void)
{
	t_tokens		*tokens;
	t_command		*cmd;
	t_env			*env;

	printf("Testing empty command...\n");
	tokens = NULL;
	env = create_dummy_env();
	
	cmd = parse_tokens_to_commands(tokens, 0, env);
	
	assert(cmd == NULL);
	
	free_dummy_env(env);
	printf("✓ Empty command test passed\n");
}

int	main(void)
{
	printf("=== Parser Module Test Suite ===\n\n");
	
	test_simple_command();
	test_command_with_output_redirect();
	test_command_with_input_redirect();
	test_pipe_commands();
	test_complex_pipeline();
	test_multiple_redirections();
	test_heredoc_parsing();
	test_empty_command();
	
	printf("\n=== All tests passed! ===\n");
	return (0);
}


