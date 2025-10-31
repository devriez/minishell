/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:11:46 by johartma          #+#    #+#             */
/*   Updated: 2025/10/29 18:50:24 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "../../libft/libft.h"
#include "../../includes/minishell.h"

char	*ft_strjoin_char(char *str, char c)
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

static char	*get_var_value(char *var_name, t_mini mini)
{
	char	*value;
	pid_t	pid;

	if (ft_strcmp(var_name, "?") == 0)
	{
		return (ft_itoa(mini.last_exit_status));
	}
	if (ft_strcmp(var_name, "$") == 0)
	{
		pid = fork();
		if (pid == 0)
			exit(0);
		else
			return (pid_to_array(pid));
	}
	if (ft_strcmp(var_name, "0") == 0)
		return (ft_strdup(mini.shell_name));
	value = get_env_var_val((mini.env), var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*extract_var_name(t_wordm str, size_t start, size_t *len)
{
	size_t			i;
	char			*name;
	t_word_flags	start_flag;

	i = start + 1;
	start_flag = str.mask[i];
	if (str.s[i] == '?' || str.s[i] == '$')
	{
		*len = 1;
		if (str.s[i] == '?')
			return (ft_strdup("?"));
		else
			return (ft_strdup("$"));
	}
	if (!ft_isalpha(str.s[i]) && str.s[i] != '_' && str.s[i] != '0')
	{
		*len = 0;
		return (NULL);
	}
	while (str.s[i] && (ft_isalnum(str.s[i]) || str.s[i] == '_') \
		&& (str.mask[i] == start_flag))
		i++;
	*len = i;
	name = ft_substr(str.s, start + 1, i - start - 1);
	return (name);
}

char	*expand_single_var(t_wordm str, size_t start, t_mini mini, 
								size_t *new_len)
{
	char	*var_name;
	char	*var_value;
	size_t	name_len;

	var_name = extract_var_name(str, start, &name_len);
	if (!var_name)
	{
		*new_len = 1;
		return (ft_strdup("$"));
	}
	var_value = get_var_value(var_name, mini);
	free(var_name);
	if (!var_value)
	{
		*new_len = 0;
		return (ft_strdup(""));
	}
	*new_len = ft_strlen(var_value);
	return (var_value);
}

int	should_expand_in_context(t_word_flags *mask, size_t pos)
{
	if (!mask)
		return (1);
	if (mask[pos] & Q_SQ)
		return (0);
	return (1);
}
