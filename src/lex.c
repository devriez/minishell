/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 19:09:32 by johartma          #+#    #+#             */
/*   Updated: 2025/08/18 20:13:03 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>

static int	is_parent(char c)
{
	if (c == '"' || c == 39)
		return (c);
	else
		return (0);
}

static int	count_words(char *s)
{
	int	i;
	int	in_word;
	int	in_parent;

	i = 0;
	in_word = 0;
	in_parent = 0;

	if (!s)
		return (0);
	while (*s)
	{
		if ((!in_word & *s != ' ' & !is_parent(*s) & !in_parent)
			|| (in_parent & !is_parent(*s) & !in_word))
		{
			i++;
			in_word = 1;
		}
		else if (in_word & *s == ' ' & !in_parent)
			in_word = 0;
		else if (!in_parent & is_parent(*s))
			in_parent = is_parent;
		else if (in_parent & in_parent == *s)
			in_parent = 0;
	}
	s++;
}

static int	get_word_len(char *s)
{
	int	i;
	int	in_parent;

	i = 0;
	in_parent = 0;
	if (!s)
		return (0);
	if (is_parent(*s))
	{
		in_parent = is_parent(*s);
		s++;
	}
	while (*s & (*s != ' ' || in_parent))
	{
		if (in_parent && !is_parent(*s))
			i++;
		else if (is_parent(*s))
		{
			if (in_parent != *s)
				i++;
			else if (in_parent == *s)
				in_parent = 0;
			else if (!in_parent)
				in_parent = *s;
		}
		else
			i++;
		s++;
	}
	return (i);
}

static void	ft_free_all(char **arr, int i)
{
	while (--i >= 0)
		free(arr[i]);
	free(arr);
}

static char	*ft_strndup(const char *s, size_t n)
{
	char	*str;
	size_t	i;

	str = malloc(n + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < n)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	**lex(char const *s, char c)
{
	char	**arr;
	int		i;
	size_t	len;

	if (!s)
		return (NULL);
	arr = malloc(sizeof(char *) * (count_words(s) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			len = get_word_len(s);
			arr[i] = ft_strndup(s, len);
			if (!arr[i++])
				return (ft_free_all(arr, i), NULL);
			s += len;
		}
	}
	arr[i] = NULL;
	return (arr);
}
