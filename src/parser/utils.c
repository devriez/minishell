/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:00:02 by johartma          #+#    #+#             */
/*   Updated: 2025/10/28 18:19:57 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_redirect_type	convert_word_type_to_redirect(t_word_type type)
{
	if (type == W_INPUT)
		return (INPUT);
	if (type == W_OUTPUT)
		return (OUTPUT);
	if (type == W_APPEND)
		return (APPEND);
	if (type == W_HEREDOC)
		return (HEREDOC);
	return (INPUT);
}

static size_t	count_pipes(const t_tokens *tokens, size_t count)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = 0;
	while (i < count)
	{
		if (tokens[i].type == W_PIPE)
			n++;
		i++;
	}
	return (n);
}

size_t	*find_pipe_positions(t_tokens *tokens, size_t count, size_t *pipe_count)
{
	size_t	*pos;
	size_t	i;
	size_t	j;

	if (!tokens || !pipe_count)
		return (NULL);
	*pipe_count = count_pipes(tokens, count);
	if (*pipe_count == 0)
		return (NULL);
	pos = malloc(sizeof(*pos) * (*pipe_count));
	if (!pos)
		return (*pipe_count = 0, NULL);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (tokens[i].type == W_PIPE)
			pos[j++] = i;
		i++;
	}
	return (pos);
}

static t_command	*free_cmd_and_args(t_command *cmd, char **all_args)
{
	if (cmd)
	{
		free(cmd->name);
		free_args_array(cmd->args);
		free(cmd);
	}
	free_args_array(all_args);
	return (NULL);
}

t_command	*parse_single_command(t_tokens *tokens, size_t start, size_t end)
{
	t_command	*cmd;
	char		**all_args;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	all_args = build_args_array(tokens, start, end);
	if (!all_args || !all_args[0])
	{
		free_args_array(all_args);
		free(cmd);
		return (NULL);
	}
	cmd->name = ft_strdup(all_args[0]);
	cmd->args = build_args_only_array(tokens, start, end);
	if (!cmd->name || !cmd->args)
		return (free_cmd_and_args(cmd, all_args));
	cmd->redirections = parse_redirections(tokens, start, end);
	cmd->next = NULL;
	free_args_array(all_args);
	return (cmd);
}
