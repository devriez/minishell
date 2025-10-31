/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/09/12 14:04:35 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

static size_t	count_tokens_from_lexed(t_tokens *lexed)
{
	size_t	count;

	count = 0;
	if (!lexed)
		return (0);
	while (lexed[count].text)
		count++;
	return (count);
}

static void	free_typed_tokens(t_tokens *tokens, size_t count)
{
	size_t	i;

	if (!tokens)
		return ;
	i = 0;
	while (i < count)
	{
		free(tokens[i].text);
		free(tokens[i].mask);
		i++;
	}
	free(tokens);
}

t_command	*parse_command_line(char *line, t_mini mini)
{
	t_tokens	*tokens;
	t_command	*commands;
	size_t		token_count;

	if (!line || !*line)
		return (NULL);
	tokens = lex(line);
	if (!tokens)
		return (NULL);
	token_count = count_tokens_from_lexed(tokens);
	tokens = get_type(tokens);
	if (!tokens)
		return (NULL);
	if (expand_tokens(tokens, token_count, mini) != 0)
	{
		free_typed_tokens(tokens, token_count);
		return (NULL);
	}
	commands = parse_tokens_to_commands(tokens, token_count, mini.env);
	free_typed_tokens(tokens, token_count);
	return (commands);
}
