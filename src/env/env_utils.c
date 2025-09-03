/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 13:13:04 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/03 14:31:57 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_correct_varname(char *name)
{
	if (!name || !*name)
		return (false);
	if (!ft_isalpha(*name) && *name != "_")
		return (false);
	name ++;
	while (*name)
	{
		if (!(ft_isalnum(*name) || *name == "_"))
			return (false);
		name ++;
	}
	return (true);
}

bool	is_env_var_exist(t_env *lockal_env, char *var_name)
{
	t_env	*current;

	current = lockal_env;
	while (current)
	{
		if (ft_strcmp(current->name, var_name) == 0)
			return (true);
		current = current->next;
	}
	return (false);
}

char	**parse_envv(char *name_eq_value)
{
	char	*equal_sign;
	char	**arr_name_value;

	arr_name_value = malloc(sizeof(char *) * 2);
	if (!arr_name_value) 
		return (NULL);
	arr_name_value[0] = NULL;
	arr_name_value[1] = NULL;
	equal_sign = ft_strchr(name_eq_value, '=');
	if (!equal_sign)
	{
		arr_name_value[0] = ft_strdup(name_eq_value);
		if (!arr_name_value[0]) 
			return (free(arr_name_value), NULL);
		return (arr_name_value);
	}
	arr_name_value[0] = ft_strndup(name_eq_value, equal_sign - name_eq_value);
	if (!arr_name_value[0])
		return (free(arr_name_value), NULL);
	arr_name_value[1] = ft_strdup(equal_sign + 1);
	if (!arr_name_value[1]) 
		return (free(arr_name_value[0]), free(arr_name_value), NULL);
	return (arr_name_value);
}
