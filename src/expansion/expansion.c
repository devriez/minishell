/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/10/29 18:48:09 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static size_t	skip_variable(t_wordm str, size_t start)
{
	size_t			i;
	t_word_flags	start_flag;

	i = start + 1;
	start_flag = str.mask[i];
	if (str.s[i] == '?' || str.s[i] == '$')
		return (i + 1);
	if (!ft_isalpha(str.s[i]) && str.s[i] != '_' && str.s[i] != '0')
		return (start + 1);
	while (str.s[i] && (ft_isalnum(str.s[i]) || str.s[i] == '_') \
		&& (str.mask[i] == start_flag))
		i++;
	return (i);
}

static void	expand_dollar(t_wordm original, size_t *i, char **result, \
	t_mini mini)
{
	char	*var_expansion;
	char	*temp;

	var_expansion = expand_single_var(original, i[0], mini, &(i[1]));
	if (!var_expansion)
	{
		free(*result);
		*result = NULL;
		return ;
	}
	temp = ft_strjoin(*result, var_expansion);
	free(*result);
	free(var_expansion);
	if (!temp)
	{
		*result = NULL;
		return ;
	}
	*result = temp;
	*i = skip_variable(original, *i);
}

static char	*build_expanded_string(char *orig, t_mini mini, t_word_flags *mask)
{
	char	*result;
	t_wordm	wordmask;
	size_t	i[2];

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i[0] = 0;
	wordmask.s = orig;
	wordmask.mask = mask;
	while (orig[i[0]])
	{
		if (orig[i[0]] == '$' && should_expand_in_context(mask, i[0]))
			expand_dollar(wordmask, i, &result, mini);
		else
			expand_alt(i, &result, orig);
		if (!result)
			return (NULL);
	}
	if (!*result)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

char	*expand_token_text(char *text, t_word_flags *mask, t_mini mini)
{
	char	*expanded;

	if (!text)
		return (NULL);
	expanded = build_expanded_string(text, mini, mask);
	return (expanded);
}

int	expand_tokens(t_tokens *tokens, size_t token_count, t_mini mini)
{
	size_t	i;
	char	*expanded_text;
	char	*old_text;

	if (!tokens)
		return (1);
	i = 0;
	while (i < token_count)
	{
		if (tokens[i].type == W_WORD)
		{
			old_text = tokens[i].text;
			expanded_text = expand_token_text(tokens[i].text, \
												tokens[i].mask, mini);
			if (!expanded_text)
				tokens[i].type = W_NONE;
			tokens[i].text = expanded_text;
			tokens[i].len = ft_strlen(expanded_text);
			tokens[i].meta.has_qoutes = 1;
			free(old_text);
		}
		i++;
	}
	return (0);
}
