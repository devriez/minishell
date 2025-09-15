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

#include "parser.h"
#include "../type/typing/typing.h"
#include "../expansion/expansion.h"
#include <stdlib.h>

static size_t	count_tokens_from_lexed(char **lexed)
{
	size_t	count;

	count = 0;
	while (lexed && lexed[count])
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
		i++;
	}
	free(tokens);
}

t_command	*parse_command_line(char *line)
{
	char		**lexed;
	t_tokens	*tokens;
	t_command	*commands;
	size_t		token_count;

	if (!line || !*line)
		return (NULL);
	lexed = lex(line);
	if (!lexed)
		return (NULL);
	token_count = count_tokens_from_lexed(lexed);
	tokens = get_type(lexed, &tokens);
	free(lexed);  // Only free the array, not the strings (now owned by tokens)
	if (!tokens)
		return (NULL);
	if (expand_tokens(tokens, token_count, NULL) != 0)
	{
		free_typed_tokens(tokens, token_count);
		return (NULL);
	}
	commands = parse_tokens_to_commands(tokens, token_count, NULL);
	free_typed_tokens(tokens, token_count);
	return (commands);
}
