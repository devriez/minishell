/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/02 18:08:02 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**parse_envv(char *name_eq_value)
{
	char	*equal_sign;
	char	**arr_name_value;

	arr_name_value = malloc(sizeof(char *) * 2);
	if (!arr_name_value) 
		return (NULL);
	equal_sign = ft_strchr(name_eq_value, '=');
	if (!equal_sign)
	{
		arr_name_value[0] = ft_strdup(name_eq_value);
		if (!arr_name_value[0]) 
			return (free(arr_name_value[0]), free(arr_name_value), NULL);
		arr_name_value[1] = NULL;
		return (arr_name_value);
	}
	arr_name_value[0] = ft_strndup(name_eq_value, equal_sign - name_eq_value);
	arr_name_value[1] = ft_strdup(equal_sign + 1);
	if (!arr_name_value[0] || !arr_name_value[1]) 
	{
		free(arr_name_value[0]);
		free(arr_name_value[1]);
		free(arr_name_value);
		return (NULL);
	}
	return (arr_name_value);
}

t_env	*env_to_list(char **envp)
{
	int		i;
	t_env	*lockal_env;
	char	**name_value;

	lockal_env = NULL;
	i = 0;
	while (envp[i])
	{
		name_value = parse_envv(envp[i]);
		if (name_value == NULL)
			return (NULL);
		if (set_env_var_from_pair(&lockal_env, name_value[0], name_value[1]))
		{
			ft_free_env_list(lockal_env);
			return (NULL);
		}
		free(name_value[0]);
		free(name_value[1]);
		free(name_value);
		i++;
	}
	return (lockal_env);
}
