/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/09/12 13:58:21 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../expansion.h"

/* Global for testing exit status */
int	g_last_exit_status = 0;

/* Mock get_env_var function for testing */
char	*get_env_var(t_env *lockal_env, char *var_name)
{
	t_env	*current;

	current = lockal_env;
	while (current)
	{
		if (strcmp(current->name, var_name) == 0)
			return (strdup(current->value));
		current = current->next;
	}
	return (NULL);
}

/* Test helper functions */

static t_env	*create_test_env(void)
{
	t_env	*env;
	t_env	*node;

	env = malloc(sizeof(t_env));
	env->name = strdup("HOME");
	env->value = strdup("/home/user");
	env->next = NULL;

	node = malloc(sizeof(t_env));
	node->name = strdup("PATH");
	node->value = strdup("/bin:/usr/bin");
	node->next = NULL;
	env->next = node;

	node = malloc(sizeof(t_env));
	node->name = strdup("USER");
	node->value = strdup("johartma");
	node->next = NULL;
	env->next->next = node;

	node = malloc(sizeof(t_env));
	node->name = strdup("EMPTY");
	node->value = strdup("");
	node->next = NULL;
	env->next->next->next = node;

	return (env);
}

static void	free_test_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
}


/* Test cases */

static void	test_basic_variable_expansion(void)
{
	t_env		*env;
	char		*result;

	printf("Testing basic variable expansion...\n");
	env = create_test_env();
	
	result = expand_token_text("$HOME", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "/home/user") == 0);
	free(result);
	
	result = expand_token_text("$USER", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "johartma") == 0);
	free(result);
	
	free_test_env(env);
	printf("✓ Basic variable expansion test passed\n");
}

static void	test_exit_status_expansion(void)
{
	t_env		*env;
	char		*result;

	printf("Testing exit status expansion...\n");
	env = create_test_env();
	g_last_exit_status = 42;
	
	result = expand_token_text("$?", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "42") == 0);
	free(result);
	
	g_last_exit_status = 0;
	result = expand_token_text("$?", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "0") == 0);
	free(result);
	
	free_test_env(env);
	printf("✓ Exit status expansion test passed\n");
}

static void	test_undefined_variable_expansion(void)
{
	t_env		*env;
	char		*result;

	printf("Testing undefined variable expansion...\n");
	env = create_test_env();
	
	result = expand_token_text("$UNDEFINED", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "") == 0);
	free(result);
	
	free_test_env(env);
	printf("✓ Undefined variable expansion test passed\n");
}

static void	test_empty_variable_expansion(void)
{
	t_env		*env;
	char		*result;

	printf("Testing empty variable expansion...\n");
	env = create_test_env();
	
	result = expand_token_text("$EMPTY", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "") == 0);
	free(result);
	
	free_test_env(env);
	printf("✓ Empty variable expansion test passed\n");
}

static void	test_mixed_content_expansion(void)
{
	t_env		*env;
	char		*result;

	printf("Testing mixed content expansion...\n");
	env = create_test_env();
	
	result = expand_token_text("Hello $USER!", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "Hello johartma!") == 0);
	free(result);
	
	result = expand_token_text("$HOME/Documents", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "/home/user/Documents") == 0);
	free(result);
	
	free_test_env(env);
	printf("✓ Mixed content expansion test passed\n");
}

static void	test_multiple_variables(void)
{
	t_env		*env;
	char		*result;

	printf("Testing multiple variables...\n");
	env = create_test_env();
	g_last_exit_status = 127;
	
	result = expand_token_text("$USER:$HOME:$?", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "johartma:/home/user:127") == 0);
	free(result);
	
	free_test_env(env);
	printf("✓ Multiple variables test passed\n");
}

static void	test_quote_context_expansion(void)
{
	t_env			*env;
	char			*result;
	t_word_flags	mask[10];

	printf("Testing quote context expansion...\n");
	env = create_test_env();
	
	/* Initialize mask - simulate single quotes context */
	for (int i = 0; i < 10; i++)
		mask[i] = Q_SQ;
	
	result = expand_token_text("$HOME", mask, env);
	assert(result != NULL);
	assert(strcmp(result, "$HOME") == 0);  /* Should not expand in single quotes */
	free(result);
	
	/* Initialize mask - simulate double quotes context */
	for (int i = 0; i < 10; i++)
		mask[i] = Q_DQ;
	
	result = expand_token_text("$HOME", mask, env);
	assert(result != NULL);
	assert(strcmp(result, "/home/user") == 0);  /* Should expand in double quotes */
	free(result);
	
	free_test_env(env);
	printf("✓ Quote context expansion test passed\n");
}

static void	test_invalid_variable_syntax(void)
{
	t_env		*env;
	char		*result;

	printf("Testing invalid variable syntax...\n");
	env = create_test_env();
	
	result = expand_token_text("$", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "$") == 0);
	free(result);
	
	result = expand_token_text("$123", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "$123") == 0);  /* Invalid var name should not expand */
	free(result);
	
	free_test_env(env);
	printf("✓ Invalid variable syntax test passed\n");
}

static void	test_token_array_expansion(void)
{
	t_env		*env;
	t_tokens	tokens[3];
	int			result;

	printf("Testing token array expansion...\n");
	env = create_test_env();
	
	/* Create test tokens */
	tokens[0].text = strdup("echo");
	tokens[0].type = W_WORD;
	tokens[0].len = 4;
	tokens[0].mask = NULL;
	
	tokens[1].text = strdup("$HOME");
	tokens[1].type = W_WORD;
	tokens[1].len = 5;
	tokens[1].mask = NULL;
	
	tokens[2].text = strdup(">");
	tokens[2].type = W_OUTPUT;
	tokens[2].len = 1;
	tokens[2].mask = NULL;
	
	result = expand_tokens(tokens, 3, env);
	assert(result == 0);
	assert(strcmp(tokens[0].text, "echo") == 0);  /* Should not change */
	assert(strcmp(tokens[1].text, "/home/user") == 0);  /* Should expand */
	assert(strcmp(tokens[2].text, ">") == 0);  /* Should not change */
	
	/* Clean up */
	free(tokens[0].text);
	free(tokens[1].text);  /* This was expanded and replaced */
	free(tokens[2].text);
	free_test_env(env);
	printf("✓ Token array expansion test passed\n");
}

static void	test_no_expansion_needed(void)
{
	t_env		*env;
	char		*result;

	printf("Testing text with no expansion needed...\n");
	env = create_test_env();
	
	result = expand_token_text("hello world", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "hello world") == 0);
	free(result);
	
	result = expand_token_text("file.txt", NULL, env);
	assert(result != NULL);
	assert(strcmp(result, "file.txt") == 0);
	free(result);
	
	free_test_env(env);
	printf("✓ No expansion needed test passed\n");
}

int	main(void)
{
	printf("=== Expansion Module Test Suite ===\n\n");
	
	test_basic_variable_expansion();
	test_exit_status_expansion();
	test_undefined_variable_expansion();
	test_empty_variable_expansion();
	test_mixed_content_expansion();
	test_multiple_variables();
	test_quote_context_expansion();
	test_invalid_variable_syntax();
	test_token_array_expansion();
	test_no_expansion_needed();
	
	printf("\n=== All tests passed! ===\n");
	return (0);
}
