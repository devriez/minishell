/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 13:13:04 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/11 13:45:31 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_correct_varname(char *name)
{
	int	i;
	
	i = 0;
	if (!name || !name[i])
		return (false);
	if (!ft_isalpha(name[i]) && name[i] != '_')
		return (false);
	i ++;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && (name[i] != '_'))
			return (false);
		i++;
	}
	return (true);
}

bool	is_env_var_exist(t_env *local_env, char *var_name)
{
	t_env	*current;

	current = local_env;
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

	arr_name_value = calloc(3, sizeof(char *));
	if (!arr_name_value) 
		return (NULL);
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

void	sort_env(t_env *local_env)
{
	int		swapped;
	t_env	*current;
	char	*temp_name;
	char	*temp_value;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = local_env;
		while (current && current->next)
		{
			if (ft_strcmp(current->name, current->next->name) > 0)
			{
				temp_name = current->name;
				temp_value = current->value;
				current->name = current->next->name;
				current->value = current->next->value;
				current->next->name = temp_name;
				current->next->value = temp_value;
				swapped = 1;
			}
			current = current->next;
		}
	}
}

void	free_node(t_env *node)
{
	if (node)
	{
		if (node->name)
			free(node->name);
		if (node->value)
			free(node->value);
		free(node);
	}
}