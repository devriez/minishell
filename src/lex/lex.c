/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 19:09:32 by johartma          #+#    #+#             */
/*   Updated: 2025/09/02 11:55:23 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../includes/minishell.h"
#include <stdlib.h>

static int	is_parent(char c)
{
	if (c == '"' || c == 39)
		return (c);
	else
		return (0);
}

static int	op_len(const char *s)
{
	if (!s || !*s)
		return (0);
	if (s[0] == '<' && s[1] == '<')
		return (2);
	if (s[0] == '>' && s[1] == '>')
		return (2);
	if (s[0] == '<' || s[0] == '>' || s[0] == '|')
		return (1);
	return (0);
}

static int  count_words(const char *s)
{
    int count = 0;
    char in_parent = 0;

    if (!s) return 0;

    while (*s) {
        // skip spaces
        while (*s == ' ') s++;
        if (!*s) break;

        // operator token?
        int opl = op_len(s);
        if (!in_parent && opl > 0) {
            count++;
            s += opl;
            continue;
        }

        // normal word (may contain quotes)
        count++;
        while (*s) {
            if (!in_parent && *s == ' ') break;

            // stop normal word when an operator starts (unless inside quotes)
            if (!in_parent) {
                int inner_opl = op_len(s);
                if (inner_opl > 0) break;
            }

            if (*s == '"' || *s == '\'') {
                if (!in_parent) in_parent = *s;
                else if (in_parent == *s) in_parent = 0;
                s++; // consume the quote
            } else {
                s++; // consume normal char
            }
        }
    }
    return count;
}

static int  get_word_span(const char *s, size_t *emit, size_t *span)
{
    char in_parent = 0;

    *emit = 0;
    *span = 0;

    // if token starts with an operator, emit it as a standalone token
    int opl = op_len(s);
    if (opl > 0) {
        *span = opl;
        *emit = opl;
        return 0;
    }

    // otherwise scan a normal word, stopping at space or operator (unless quoted)
    while (s[*span] && (in_parent || s[*span] != ' ')) {
        if (!in_parent) {
            int inner_opl = op_len(&s[*span]);
            if (inner_opl > 0) break; // end of this word before the operator
        }
        if (s[*span] == '"' || s[*span] == '\'') {
            if (!in_parent || in_parent == s[*span]) {
                if (!in_parent) in_parent = s[*span];
                else in_parent = 0;
                (*span)++; // do not count quote in emit
            } else {
                (*emit)++;
                (*span)++;
            }
        } else {
            (*emit)++;
            (*span)++;
        }
    }
    if (in_parent) return -1; // unclosed quote
    return 0;
}

void	lex_free(char **arr, int i)
{
	while (--i >= 0)
		free(arr[i]);
	free(arr);
}


static char	*dup_word_stripping(const char *s, size_t span, size_t emit)
{
	char	*out;
	size_t	i[2];
	char	in_parent;

	i[0] = 0;
	i[1] = 0;
	out = malloc(emit + 1);
	if (!out)
		return (NULL);
	in_parent = 0;

	while (i[0] < span)
	{
		if (is_parent(s[i[0]]))
		{
			if (!in_parent)
			{
				in_parent = s[i[0]];
				i[0]++;
				continue ;
			}
			if (in_parent == s[i[0]])
			{
				in_parent = 0;
				i[0]++;
				continue ;
			}
		}
		out[i[1]++] = s[i[0]];
		i[0]++;
	}
	out[i[1]] = '\0';
	return (out);
}

char	**lex(char const *s)
{
	char	**arr;
	int		i;
	size_t	emit;
	size_t	span;

	if (!s)
		return (NULL);
	arr = malloc(sizeof(char *) * (count_words(s) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == ' ')
			s++;
		if (!*s)
			break ;
		emit = 0;
		span = 0;
		if (get_word_span(s, &emit, &span) < 0)
			return (lex_free(arr, i), NULL);
		arr[i] = dup_word_stripping(s, span, emit);
		if (!arr[i++])
			return (lex_free(arr, i), NULL);
		s += span;
	}
	arr[i] = NULL;
	return (arr);
}
