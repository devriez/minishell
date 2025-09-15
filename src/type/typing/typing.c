/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:08:11 by johartma          #+#    #+#             */
/*   Updated: 2025/09/12 13:51:01 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "typing.h"
#include <stdlib.h>
#include <string.h>
#include "../../../libft/libft.h"

static size_t	get_lislen(char **lexed)
{
	size_t	len;

	if (!lexed)
		return (0);
	len = 0;
	while (lexed[len])
		len++;
	return (len);
}

static void	cleanup(void)
{
	write(STDERR_FILENO, "Error: Memory allocation failed\n", 32);
	exit(1);
}

static void	init_token_meta(t_tokens *token)
{
	token->meta.has_qoutes = 0;
	token->meta.is_here_delim = 0;
	token->meta.delim_qouted = 0;
	token->mask = NULL;
	token->len = ft_strlen(token->text);
}

static void	mark_heredoc_delimiters(t_tokens *tokens, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (tokens[i].type == W_HEREDOC && i + 1 < len)
			tokens[i + 1].meta.is_here_delim = 1;
		i++;
	}
}

t_tokens	*get_type(char **lexed, t_tokens **tokens)
{
	size_t	len_tokens;
	size_t	i;

	len_tokens = get_lislen(lexed);
	*tokens = ft_calloc(len_tokens, sizeof(t_tokens));
	if (!(*tokens))
		cleanup();
	i = 0;
	while (i < len_tokens)
	{
		(*tokens)[i].text = lexed[i];
		init_token_meta(&(*tokens)[i]);
		if (strcmp(lexed[i], "|") == 0)
			(*tokens)[i].type = W_PIPE;
		else if (strcmp(lexed[i], "<") == 0)
			(*tokens)[i].type = W_INPUT;
		else if (strcmp(lexed[i], ">") == 0)
			(*tokens)[i].type = W_OUTPUT;
		else if (strcmp(lexed[i], ">>") == 0)
			(*tokens)[i].type = W_APPEND;
		else if (strcmp(lexed[i], "<<") == 0)
			(*tokens)[i].type = W_HEREDOC;
		else
			(*tokens)[i].type = W_WORD;
		i++;
	}
	mark_heredoc_delimiters(*tokens, len_tokens);
	return (*tokens);
}

