/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 00:00:00 by johartma          #+#    #+#             */
/*   Updated: 2025/09/15 19:16:51 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include <stdlib.h>
#include <string.h>
#include "../../libft/libft.h"

extern int	g_last_exit_status;

static char	*ft_strjoin_char(char *str, char c)
{
	char	*result;
	size_t	len;
	size_t	i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = str[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}

static char	*get_var_value(char *var_name, t_env *env)
{
	char	*value;

	if (strcmp(var_name, "?") == 0)
	{
		return (ft_itoa(g_last_exit_status));
	}
	value = get_env_var_val(env, var_name);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

static char	*extract_var_name(char *str, size_t *len)
{
	size_t	i;
	char	*name;

	if (*str == '?')
	{
		*len = 1;
		return (ft_strdup("?"));
	}
	if (!ft_isalpha(*str) && *str != '_')
	{
		*len = 0;
		return (NULL);
	}
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*len = i;
	name = ft_substr(str, 0, i);
	return (name);
}

static char	*expand_single_var(char *str, size_t start, t_env *env, 
								size_t *new_len)
{
	char	*var_name;
	char	*var_value;
	size_t	name_len;

	var_name = extract_var_name(str + start + 1, &name_len);
	if (!var_name)
	{
		*new_len = 1;
		return (ft_strdup("$"));
	}
	var_value = get_var_value(var_name, env);
	free(var_name);
	if (!var_value)
	{
		*new_len = 0;
		return (ft_strdup(""));
	}
	*new_len = ft_strlen(var_value);
	return (var_value);
}

static int	should_expand_in_context(t_word_flags *mask, size_t pos)
{
	if (!mask)
		return (1);
	if (mask[pos] & Q_SQ)
		return (0);
	return (1);
}

static size_t	skip_variable(char *str, size_t start)
{
	size_t	i;

	i = start + 1;
	if (str[i] == '?')
		return (i + 1);
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (start + 1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

static char	*build_expanded_string(char *original, t_env *env, 
									t_word_flags *mask)
{
	char	*result;
	char	*temp;
	char	*var_expansion;
	size_t	i;
	size_t	var_len;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (original[i])
	{
		if (original[i] == '$' && should_expand_in_context(mask, i))
		{
			var_expansion = expand_single_var(original, i, env, &var_len);
			if (!var_expansion)
			{
				free(result);
				return (NULL);
			}
			temp = ft_strjoin(result, var_expansion);
			free(result);
			free(var_expansion);
			if (!temp)
				return (NULL);
			result = temp;
			i = skip_variable(original, i);
		}
		else
		{
			temp = ft_strjoin_char(result, original[i]);
			free(result);
			if (!temp)
				return (NULL);
			result = temp;
			i++;
		}
	}
	return (result);
}

char	*expand_token_text(char *text, t_word_flags *mask, t_env *env)
{
	char	*expanded;

	if (!text)
		return (NULL);
	expanded = build_expanded_string(text, env, mask);
	return (expanded);
}

int	expand_tokens(t_tokens *tokens, size_t token_count, t_env *env)
{
	size_t	i;
	char	*expanded_text;
	char	*old_text;

	if (!tokens || !env)
		return (1);
	i = 0;
	while (i < token_count)
	{
		if (tokens[i].type == W_WORD)
		{
			old_text = tokens[i].text;
			expanded_text = expand_token_text(tokens[i].text, 
												tokens[i].mask, env);
			if (!expanded_text)
				return (1);
			tokens[i].text = expanded_text;
			tokens[i].len = ft_strlen(expanded_text);
			tokens[i].meta.has_qoutes = 1;
			free(old_text);
		}
		i++;
	}
	return (0);
}
