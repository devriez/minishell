/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/09/15 19:47:46 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../includes/minishell.h"

/* Main parser function - converts token array to command structures */
t_command	*parse_tokens_to_commands(t_tokens *tokens, size_t count, t_env *env);

/* Parse a single command from tokens (between pipes) */
t_command	*parse_single_command(t_tokens *tokens, size_t start, size_t end);

/* Build arguments array from word tokens */
char		**build_args_array(t_tokens *tokens, size_t start, size_t end);

/* Parse redirections and build redirect list */
t_redirect	*parse_redirections(t_tokens *tokens, size_t start, size_t end);

/* Find pipe positions to split commands */
size_t		*find_pipe_positions(t_tokens *tokens, size_t count, size_t *pipe_count);

/* Main entry point - complete parsing pipeline (defined in separate module) */
t_command	*parse_command_line(char *line, t_env *env);

#endif
