/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 19:09:32 by johartma          #+#    #+#             */
/*   Updated: 2025/10/28 20:43:00 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void	process_word_chars(t_lex_help mespa, char *out,
				t_word_flags *mask, t_meta *meta)
{
	size_t	i;
	size_t	j;
	char	in_parent;
	char	c[2];

	init_state(meta, &i, &j, &in_parent);
	while (i < mespa.span)
	{
		if (!handle_quote(mespa.s[i], &in_parent, meta))
		{
			c[0] = mespa.s[i];
			c[1] = in_parent;
			copy_and_mark(c, out, mask, &j);
		}
		i++;
	}
	out[j] = '\0';
}

static char	*dup_word_stripping(t_lex_help mespa, size_t emit,
				t_word_flags **mask_out, t_meta *meta_out)
{
	char			*out;
	t_word_flags	*mask;

	out = malloc(emit + 1);
	mask = malloc(sizeof(t_word_flags) * emit);
	if (!out || !mask)
	{
		free(out);
		free(mask);
		return (NULL);
	}
	process_word_chars(mespa, out, mask, meta_out);
	*mask_out = mask;
	return (out);
}

static t_tokens	*init_tokens(const char *s, int *count)
{
	t_tokens	*tokens;

	*count = count_words(s);
	tokens = calloc(*count + 1, sizeof(t_tokens));
	if (!tokens)
		return (NULL);
	return (tokens);
}

static int	process_word(char **s, t_tokens *tokens, int *i)
{
	size_t			emit;
	t_word_flags	*mask;
	t_meta			meta;
	char			*word;
	t_lex_help		mespa;

	emit = 0;
	mespa.span = 0;
	if (get_word_span(*s, &emit, &(mespa.span)) < 0)
		return (-1);
	mask = NULL;
	mespa.s = *s;
	word = dup_word_stripping(mespa, emit, &mask, &meta);
	if (!word)
		return (-1);
	tokens[*i].text = word;
	tokens[*i].mask = mask;
	tokens[*i].len = emit;
	tokens[*i].meta = meta;
	(*i)++;
	*s += mespa.span;
	return (0);
}

t_tokens	*lex(char *s)
{
	t_tokens	*tokens;
	int			n;
	int			i;

	if (!s)
		return (NULL);
	tokens = init_tokens(s, &n);
	if (!tokens)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == ' ' || *s == '	')
			s++;
		if (!*s)
			break ;
		if (process_word(&s, tokens, &i) < 0)
		{
			lex_free(tokens, i);
			return (NULL);
		}
	}
	return (tokens);
}
