/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_envv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:11:40 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/02 18:11:13 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_node(char *name, char *value)
{
	t_env	*node;

	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->name = ft_strdup(name);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = ft_strdup("");
	node->next = NULL;
	if (!node->name || !node->value)
	{
		free(node->name);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

void	add_node_to_end(t_env **head, t_env	*new_node)
{
	t_env	*last;

	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	last = *head;
	while (last->next != NULL)
		last = last->next;
	last->next = new_node;
	return ;
}

int	rewrite_env_var(t_env *lockal_env, char *var_name, char *var_value)
{
	t_env	*current;

	current = lockal_env;
	while (current)
	{
		if (ft_strcmp(current->name, var_name) == 0)
		{
			if (var_value)
			{
				free(current->value);
				current->value = ft_strdup(var_value);
				if (!current->value)
					return (1);
				return (0);
			}
			else
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	set_env_var_from_pair(t_env **lockal_env, char *var_name, char *var_value)
{
	t_env	*new_node;

	if (!is_correct_varname(var_name))
	{
		printf("create new envv: not a valid name %s", var_name);
		return (1);
	}
	if (is_env_var_exist(*lockal_env, var_name))
		return (rewrite_env_var(*lockal_env, var_name, var_value));
	else
	{
		new_node = create_env_node(var_name, var_value);
		if (!new_node)
			return (1);
		add_node_to_end(lockal_env, new_node);
		return (0);
	}
}
