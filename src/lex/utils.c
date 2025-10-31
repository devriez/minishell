/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:56:10 by johartma          #+#    #+#             */
/*   Updated: 2025/10/28 17:18:21 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	lex_free(t_tokens *arr, int i)
{
	while (--i >= 0)
		free(arr[i].text);
	free(arr);
}

void	init_state(t_meta *meta, size_t *i, size_t *j, char *in_parent)
{
	*i = 0;
	*j = 0;
	*in_parent = 0;
	ft_memset(meta, 0, sizeof(t_meta));
}

int	handle_quote(char c, char *in_parent, t_meta *meta)
{
	if (is_parent(c) && (!*in_parent || *in_parent == c))
	{
		if (!*in_parent)
		{
			*in_parent = c;
			meta->has_qoutes = 1;
		}
		else
			*in_parent = 0;
		return (1);
	}
	return (0);
}

static void	set_mask_char(char in_parent, t_word_flags *mask, size_t j)
{
	if (in_parent == '\'')
		mask[j] = Q_SQ;
	else if (in_parent == '"')
		mask[j] = Q_DQ;
	else
		mask[j] = Q_NONE;
}

void	copy_and_mark(char c[2], char *out,
				t_word_flags *mask, size_t *j)
{
	out[*j] = c[0];
	set_mask_char(c[1], mask, *j);
	(*j)++;
}
