/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:00:02 by johartma          #+#    #+#             */
/*   Updated: 2025/10/28 18:11:03 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**alloc_empty_vec(void)
{
	char	**v;

	v = malloc(sizeof(*v));
	if (!v)
		return (NULL);
	v[0] = NULL;
	return (v);
}

static char	**dup_excluding_first(char **src, size_t n)
{
	char	**dst;
	size_t	i;

	dst = malloc(sizeof(*dst) * n);
	if (!dst)
		return (NULL);
	i = 0;
	while (i + 1 < n)
	{
		dst[i] = ft_strdup(src[i + 1]);
		if (!dst[i])
			return (free_args_array(dst), NULL);
		i++;
	}
	dst[i] = NULL;
	return (dst);
}

char	**build_args_only_array(t_tokens *tokens, size_t start, size_t end)
{
	char	**all;
	char	**only;
	size_t	wc;

	all = build_args_array(tokens, start, end);
	if (!all || !all[0])
		return (free_args_array(all), NULL);
	wc = ptr_count(all);
	if (wc == 1)
		return (free_args_array(all), alloc_empty_vec());
	only = dup_excluding_first(all, wc);
	if (!only)
		return (free_args_array(all), NULL);
	free_args_array(all);
	return (only);
}

t_redirect	*create_redirect_node(t_redirect_type type, char *file, \
	int fd)
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

void	add_redirect_to_list(t_redirect **head, t_redirect *new_node)
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
