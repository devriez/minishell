/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/09/12 14:11:43 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include "../../libft/libft.h"

static int				is_redirection_target(t_tokens *tokens, size_t pos);
static t_redirect_type	convert_word_type_to_redirect(t_word_type type);

static void	free_args_array(char **args)
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

static size_t	count_word_tokens(t_tokens *tokens, size_t start, size_t end)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = start;
	while (i < end)
	{
		if (tokens[i].type == W_WORD && !is_redirection_target(tokens, i))
			count++;
		i++;
	}
	return (count);
}

static int	is_redirection_target(t_tokens *tokens, size_t pos)
{
	if (pos == 0)
		return (0);
	if (tokens[pos - 1].type == W_INPUT || tokens[pos - 1].type == W_OUTPUT ||
		tokens[pos - 1].type == W_APPEND || tokens[pos - 1].type == W_HEREDOC)
		return (1);
	return (0);
}

static char	**allocate_args_array(size_t word_count)
{
	char	**args;

	args = malloc(sizeof(char *) * (word_count + 1));
	return (args);
}

static int	add_word_to_args(char **args, char *text, size_t index)
{
	args[index] = ft_strdup(text);
	if (!args[index])
		return (0);
	return (1);
}

char	**build_args_array(t_tokens *tokens, size_t start, size_t end)
{
	char	**args;
	size_t	word_count;
	size_t	arg_index;
	size_t	i;

	word_count = count_word_tokens(tokens, start, end);
	args = allocate_args_array(word_count);
	if (!args)
		return (NULL);
	arg_index = 0;
	i = start;
	while (i < end && arg_index < word_count)
	{
		if (tokens[i].type == W_WORD && !is_redirection_target(tokens, i))
		{
			if (!add_word_to_args(args, tokens[i].text, arg_index))
			{
				free_args_array(args);
				return (NULL);
			}
			arg_index++;
		}
		i++;
	}
	args[arg_index] = NULL;
	return (args);
}

static t_redirect	*create_redirect_node(t_redirect_type type, char *file, int fd)
{
	t_redirect	*node;

	node = malloc(sizeof(t_redirect));
	if (!node)
		return (NULL);
	node->type = type;
	node->file = file;
	node->heredoc_fd = fd;
	node->next = NULL;
	return (node);
}

static void	add_redirect_to_list(t_redirect **head, t_redirect *new_node)
{
	t_redirect	*current;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

static int	is_redirect_token(t_word_type type)
{
	return (type == W_INPUT || type == W_OUTPUT ||
			type == W_APPEND || type == W_HEREDOC);
}

static t_redirect	*create_heredoc_redirect(t_redirect_type type)
{
	return (create_redirect_node(type, NULL, -1));
}

static t_redirect	*create_file_redirect(t_redirect_type type, char *filename)
{
	char	*file_copy;

	file_copy = ft_strdup(filename);
	return (create_redirect_node(type, file_copy, -1));
}

static t_redirect	*process_redirect_token(t_tokens *tokens, size_t i, size_t end)
{
	t_redirect_type	type;

	type = convert_word_type_to_redirect(tokens[i].type);
	if (tokens[i].type == W_HEREDOC && tokens[i + 1].meta.is_here_delim)
		return (create_heredoc_redirect(type));
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

static t_redirect_type	convert_word_type_to_redirect(t_word_type type)
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

size_t	*find_pipe_positions(t_tokens *tokens, size_t count, size_t *pipe_count)
{
	size_t	*pipe_positions;
	size_t	pipes_found;
	size_t	i;

	pipes_found = 0;
	i = 0;
	while (i < count)
	{
		if (tokens[i].type == W_PIPE)
			pipes_found++;
		i++;
	}
	*pipe_count = pipes_found;
	if (pipes_found == 0)
		return (NULL);
	pipe_positions = malloc(sizeof(size_t) * pipes_found);
	if (!pipe_positions)
		return (NULL);
	pipes_found = 0;
	i = 0;
	while (i < count)
	{
		if (tokens[i].type == W_PIPE)
		{
			pipe_positions[pipes_found] = i;
			pipes_found++;
		}
		i++;
	}
	return (pipe_positions);
}

t_command	*parse_single_command(t_tokens *tokens, size_t start, size_t end)
{
	t_command	*cmd;
	char		**args;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	args = build_args_array(tokens, start, end);
	if (!args || !args[0])
	{
		free(cmd);
		free_args_array(args);
		return (NULL);
	}
	cmd->name = ft_strdup(args[0]);
	cmd->args = args;
	cmd->redirections = parse_redirections(tokens, start, end);
	cmd->next = NULL;
	if (!cmd->name)
	{
		free_args_array(cmd->args);
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

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
		new_cmd = parse_single_command(tokens, start, 
			get_segment_end(pipe_pos, i, pipe_count, total_count));
		if (!new_cmd)
			return (first_cmd);
		link_command_to_chain(&first_cmd, &current_cmd, new_cmd);
		start = get_segment_end(pipe_pos, i, pipe_count, total_count) + 1;
		i++;
	}
	return (first_cmd);
}

t_command	*parse_tokens_to_commands(t_tokens *tokens, size_t count, t_env *env)
{
	size_t		*pipe_positions;
	size_t		pipe_count;
	t_command	*result;

	(void)env;
	if (!tokens || count == 0)
		return (NULL);
	pipe_positions = find_pipe_positions(tokens, count, &pipe_count);
	if (pipe_count == 0)
		result = parse_single_pipeline(tokens, count);
	else
	{
		result = parse_pipe_chain(tokens, pipe_positions, pipe_count, count);
		free(pipe_positions);
	}
	return (result);
}
