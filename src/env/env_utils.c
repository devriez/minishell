/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 13:13:04 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/30 13:13:49 by amoiseik         ###   ########.fr       */
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