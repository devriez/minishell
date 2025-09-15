/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/09/12 13:56:39 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "../includes/minishell.h"

/* Main expansion function - expands variables in token array */
int		expand_tokens(t_tokens *tokens, size_t token_count, t_env *env);

/* Expand variables in a single token based on quote context */
char	*expand_token_text(char *text, t_word_flags *mask, t_env *env);

#endif


