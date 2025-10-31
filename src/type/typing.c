/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:08:11 by johartma          #+#    #+#             */
/*   Updated: 2025/10/24 12:26:25 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>
#include <string.h>
#include "../libft/libft.h"

static size_t	get_tokens_len(t_tokens *tokens)
{
	size_t	len;

	len = 0;
	if (!tokens)
		return (0);
	while (tokens[len].text)
		len++;
	return (len);
}

static void	init_token_meta(t_tokens *token)
{
	token->meta.has_qoutes = 0;
	token->meta.is_here_delim = 0;
	token->meta.delim_qouted = 0;
	token->len = strlen(token->text);
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

static t_word_type	get_word_type(const char *text)
{
	if (strcmp(text, "|") == 0)
		return (W_PIPE);
	else if (strcmp(text, "<") == 0)
		return (W_INPUT);
	else if (strcmp(text, ">") == 0)
		return (W_OUTPUT);
	else if (strcmp(text, ">>") == 0)
		return (W_APPEND);
	else if (strcmp(text, "<<") == 0)
		return (W_HEREDOC);
	else
		return (W_WORD);
}

t_tokens	*get_type(t_tokens *tokens)
{
	size_t	len_tokens;
	size_t	i;

	if (!tokens)
		return (NULL);
	len_tokens = get_tokens_len(tokens);
	i = 0;
	while (i < len_tokens)
	{
		init_token_meta(&tokens[i]);
		tokens[i].type = get_word_type(tokens[i].text);
		i++;
	}
	mark_heredoc_delimiters(tokens, len_tokens);
	return (tokens);
}
