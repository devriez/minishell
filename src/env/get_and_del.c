/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_and_del.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:22:53 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/11 13:46:00 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var_val(t_env *local_env, char *var_name)
{
	t_env	*env_var;

	env_var = local_env;
	while (env_var)
	{
		if (ft_strcmp(env_var->name, var_name) == 0)
			return (env_var->value);
		else
			env_var = env_var->next;
	}
	return (NULL);
}

void	remove_envv(t_env **local_env, char *var_name)
{
	t_env	*current;
	t_env	*prev;
	
	current = *local_env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, var_name) == 0)
		{
			if (prev == NULL)
				*local_env = current->next;
			else
				prev->next = current->next;
			free_node(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
