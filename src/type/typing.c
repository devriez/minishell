/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:08:11 by johartma          #+#    #+#             */
/*   Updated: 2025/09/02 11:12:41 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>
#include "../libft/libft.h"

t_tokens	*get_type(char **lexed, t_tokens **tokens)
{
	size_t	len_tokens;
	size_t	i;

	len_tokens = get_lislen(lexed);
	*tokens = ft_calloc(len_tokens, sizeof(t_tokens));
	if (!(*tokens))
		cleanup();
	i = 0;
	while (i < len_tokens)
	{
		(*tokens)[i].text = lexed[i];
		if (strcmp(lexed[i], "|") == 0)
			(*tokens)[i].type = PIPE;
		else if (strcmp(lexed[i], "<") == 0)
			(*tokens)[i].type = INPUT;
		else if (strcmp(lexed[i] == ">"))
			(*tokens)[i].type = OUTPUT;
		else if (strcmp(lexed[i] == ">>"))
			(*tokens)[i].type = APPEND;
		else if (strcmp(lexed[i] == "<<"))
		{
			(*tokens)[i].type = HEREDOC;
			if (i + 1 < len_tokens)
				(*tokens)[i + 1].meta.is_here_delim = 1;
		}
		else
			(*tokens)[i].type = WORD;
		i++;
	}
	return (*tokens);
}

