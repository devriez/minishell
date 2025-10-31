/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:00:02 by johartma          #+#    #+#             */
/*   Updated: 2025/10/28 18:20:05 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_args_array(char **args)
{
	size_t	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

size_t	count_word_tokens(t_tokens *tokens, size_t start, size_t end)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = start;
	while (i < end)
	{
		if ((tokens[i].type == W_WORD) && !is_redirection_target(tokens, i))
			count++;
		i++;
	}
	return (count);
}

int	is_redirection_target(t_tokens *tokens, size_t pos)
{
	if (pos == 0)
		return (0);
	if (tokens[pos - 1].type == W_INPUT || tokens[pos - 1].type == W_OUTPUT \
		|| tokens[pos - 1].type == W_APPEND \
		|| tokens[pos - 1].type == W_HEREDOC)
		return (1);
	return (0);
}

char	**allocate_args_array(size_t word_count)
{
	char	**args;

	args = malloc(sizeof(char *) * (word_count + 1));
	return (args);
}
