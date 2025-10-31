/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:56:10 by johartma          #+#    #+#             */
/*   Updated: 2025/10/28 17:18:36 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	skip_spaces(const char **s)
{
	while (**s == ' ')
		(*s)++;
}

static int	try_consume_op(const char **s, int *count, char in_parent)
{
	int	opl;

	if (in_parent)
		return (0);
	opl = op_len(*s);
	if (opl <= 0)
		return (0);
	(*count)++;
	*s += opl;
	return (1);
}

static int	should_end_word(const char *s, char in_parent)
{
	if (!*s)
		return (1);
	if (!in_parent && *s == ' ')
		return (1);
	if (!in_parent && op_len(s) > 0)
		return (1);
	return (0);
}

static void	update_parent(const char **s, char *in_parent)
{
	if (is_parent(**s))
	{
		if (!*in_parent)
			*in_parent = **s;
		else if (*in_parent == **s)
			*in_parent = 0;
	}
}

int	count_words(const char *s)
{
	int		count;
	char	in_parent;

	count = 0;
	in_parent = 0;
	if (!s)
		return (0);
	while (*s)
	{
		skip_spaces(&s);
		if (!*s)
			break ;
		if (try_consume_op(&s, &count, in_parent))
			continue ;
		count++;
		while (!should_end_word(s, in_parent))
		{
			update_parent(&s, &in_parent);
			s++;
		}
	}
	return (count);
}
