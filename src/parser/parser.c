/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/10/28 18:19:39 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <string.h>
#include "../../libft/libft.h"

static void	link_command_to_chain(t_command **first, t_command **current,
							t_command *new_cmd)
{
	if (!*first)
	{
		*first = new_cmd;
		*current = *first;
	}
	else
	{
		(*current)->next = new_cmd;
		*current = new_cmd;
	}
}

static t_command	*parse_single_pipeline(t_tokens *tokens, size_t count)
{
	return (parse_single_command(tokens, 0, count));
}

static size_t	get_segment_end(size_t *pipe_pos, size_t i, 
							size_t pipe_count, size_t total_count)
{
	if (i < pipe_count)
		return (pipe_pos[i]);
	return (total_count);
}

static t_command	*parse_pipe_chain(t_tokens *tokens, size_t *pipe_pos,
								size_t pipe_count, size_t total_count)
{
	t_command	*first_cmd;
	t_command	*current_cmd;
	t_command	*new_cmd;
	size_t		start;
	size_t		i;

	first_cmd = NULL;
	current_cmd = NULL;
	start = 0;
	i = 0;
	while (i <= pipe_count)
	{
		new_cmd = parse_single_command(tokens, start, \
			get_segment_end(pipe_pos, i, pipe_count, total_count));
		if (!new_cmd)
			return (first_cmd);
		link_command_to_chain(&first_cmd, &current_cmd, new_cmd);
		start = get_segment_end(pipe_pos, i, pipe_count, total_count) + 1;
		i++;
	}
	return (first_cmd);
}

t_command	*parse_tokens_to_commands(t_tokens *tokens, \
	size_t count, t_env *env)
{
	size_t		*pipe_positions;
	size_t		pipe_count;
	t_command	*result;

	(void)env;
	if (!tokens || count == 0)
		return (NULL);
	pipe_positions = find_pipe_positions(tokens, count, &pipe_count);
	if (pipe_count == 0)
	{
		result = parse_single_pipeline(tokens, count);
	}
	else
	{
		result = parse_pipe_chain(tokens, pipe_positions, pipe_count, count);
		free(pipe_positions);
	}
	return (result);
}
