/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:56:10 by johartma          #+#    #+#             */
/*   Updated: 2025/10/30 14:05:23 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	set_leading_op(const char *s, size_t *emit, size_t *span)
{
	int	opl;

	opl = op_len(s);
	if (opl <= 0)
		return (0);
	*span = opl;
	*emit = opl;
	return (1);
}

static int	should_break_on_op(const char *s, size_t off, char in_parent)
{
	int	inner_opl;

	if (in_parent)
		return (0);
	inner_opl = op_len(&s[off]);
	return (inner_opl > 0);
}

static void	update_parent_and_span(char c, char *in_parent, size_t *span)
{
	if (!is_parent(c))
		return ;
	if (!*in_parent)
		*in_parent = c;
	else if (*in_parent == c)
		*in_parent = 0;
	(*span)++;
}

static void	consume_char(size_t *emit, size_t *span)
{
	(*emit)++;
	(*span)++;
}

int	get_word_span(const char *s, size_t *emit, size_t *span)
{
	char	in_parent;

	*emit = 0;
	*span = 0;
	in_parent = 0;
	if (set_leading_op(s, emit, span))
		return (0);
	while (s[*span] && (in_parent || s[*span] != ' '))
	{
		if (should_break_on_op(s, *span, in_parent))
			break ;
		if ((is_parent(s[*span]) && (!in_parent || in_parent == s[*span])))
			update_parent_and_span(s[*span], &in_parent, span);
		else
			consume_char(emit, span);
	}
	if (in_parent)
		return (-1);
	return (0);
}
