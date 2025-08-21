/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 19:09:32 by johartma          #+#    #+#             */
/*   Updated: 2025/08/21 16:20:51 by johartma         ###   ########.fr       */
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

static int	count_words(const char *s)
{
	int	i[3];

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	if (!s)
		return (0);
	while (*s)
	{
		if (!i[2] && *s == ' ')
			i[1] = 0;
		else
		{
			if (!i[1])
			{
				i[0]++;
				i[1] = 1;
			}
			if (is_parent(*s))
			{
				if (!i[2])
					i[2] = *s;
				else if (i[2] == *s)
					i[2] = 0;
			}
		}
		s++;
	}
	return (i[0]);
}

static int	get_word_span(const char *s, size_t *emit, size_t *span)
{
	char	in_parent;

	in_parent = 0;
	while (s[*span] && (in_parent || s[*span] != ' '))
	{
		if (s[*span] == '"' || s[*span] == '\'')
		{
			if (!in_parent || in_parent == s[*span])
			{
				if (!in_parent)
					in_parent = s[*span];
				else
					in_parent = 0;
				(*span)++;
			}
		}
		else
		{
			(*emit)++;
			(*span)++;
		}
	}
	if (in_parent)
		return (-1);
	return (0);
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
