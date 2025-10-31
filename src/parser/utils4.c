/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:00:02 by johartma          #+#    #+#             */
/*   Updated: 2025/10/28 18:12:24 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	add_word_to_args(char **args, char *text, size_t index)
{
	args[index] = ft_strdup(text);
	if (!args[index])
		return (0);
	return (1);
}

static char	**fail_free(char **args)
{
	free_args_array(args);
	return (NULL);
}

char	**build_args_array(t_tokens *tokens, size_t start, size_t end)
{
	char	**args;
	size_t	capacity;
	size_t	i;
	size_t	j;

	if (!tokens || start > end)
		return (NULL);
	capacity = count_word_tokens(tokens, start, end);
	args = allocate_args_array(capacity);
	if (!args)
		return (NULL);
	i = start;
	j = 0;
	while (i < end && j < capacity)
	{
		if (tokens[i].type == W_WORD && !is_redirection_target(tokens, i))
		{
			if (!add_word_to_args(args, tokens[i].text, j))
				return (fail_free(args));
			j++;
		}
		i++;
	}
	args[j] = NULL;
	return (args);
}

size_t	ptr_count(char **v)
{
	size_t	i;

	i = 0;
	while (v && v[i])
		i++;
	return (i);
}
