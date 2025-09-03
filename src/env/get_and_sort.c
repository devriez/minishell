/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:22:53 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/29 19:09:33 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var(t_env *lockal_env, char *var_name)
{
	int		i;
	t_env	*env_var;

	i = 0;
	env_var = lockal_env;
	while (env_var)
	{
		if (ft_strcmp(env_var->name, var_name) == 0)
			return (ft_strdup(env_var->value));
		else
			env_var = env_var->next;
	}
	return (NULL);
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
