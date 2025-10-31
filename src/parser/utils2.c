/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:00:02 by johartma          #+#    #+#             */
/*   Updated: 2025/10/28 18:05:02 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_redirect_token(t_word_type type)
{
	return (type == W_INPUT || type == W_OUTPUT || \
			type == W_APPEND || type == W_HEREDOC);
}

static t_redirect	*create_file_redirect(t_redirect_type type, char *filename)
{
	char	*file_copy;

	file_copy = ft_strdup(filename);
	return (create_redirect_node(type, file_copy, -1));
}

static t_redirect	*process_redirect_token(t_tokens *tokens, size_t i, \
	size_t end)
{
	t_redirect_type	type;
	t_redirect		*redirection;

	type = convert_word_type_to_redirect(tokens[i].type);
	if (tokens[i].type == W_HEREDOC && tokens[i + 1].meta.is_here_delim)
	{
		redirection = malloc(sizeof(t_redirect));
		if (!redirection)
			return (NULL);
		*redirection = read_heredoc(tokens[i + 1].text, type);
		if (redirection->heredoc_fd != -1)
			return (redirection);
		else
			return (NULL);
	}
	else if (i + 1 < end && tokens[i + 1].type == W_WORD)
		return (create_file_redirect(type, tokens[i + 1].text));
	return (NULL);
}

t_redirect	*parse_redirections(t_tokens *tokens, size_t start, size_t end)
{
	t_redirect	*redirections;
	t_redirect	*new_redirect;
	size_t		i;

	redirections = NULL;
	i = start;
	while (i < end)
	{
		if (is_redirect_token(tokens[i].type))
		{
			new_redirect = process_redirect_token(tokens, i, end);
			if (new_redirect)
				add_redirect_to_list(&redirections, new_redirect);
		}
		i++;
	}
	return (redirections);
}
