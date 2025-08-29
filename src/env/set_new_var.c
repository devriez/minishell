/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_new_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:11:40 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/29 22:58:54 by amoiseik         ###   ########.fr       */
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

t_env	*create_new_node(char *name, char *value)
{
	t_env	*new_node;

	if (!is_correct_varname(name))
	{
		printf("create new envvariable: not a valid identifier %s", name);
		return (NULL);
	}

	new_node = malloc(sizeof(*new_node));
	if (!new_node)
		return (NULL);
	new_node->name = ft_strdup(name);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (!new_node->name || !new_node->value)
	{
		free(new_node->name);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	return (new_node);
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

bool	is_env_var(t_env *lockal_env, char *var_name)
{
	current = *lockal_env;
	while (current)
	{
		if (ft_strcmp(current->name, var_name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(var_value);
			if (!current->value)
				return (1);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	rewrite_env_var(t_env **lockal_env, char *var_name, char *var_value)
{
	t_env	*current;

	current = *lockal_env;
	while (current)
	{
		if (ft_strcmp(current->name, var_name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(var_value);
			if (!current->value)
				return (1);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	set_env_var(t_env **lockal_env, char *var_name, char *var_value)
{
	t_env	*new_node;

	if (!rewrite_env_var(lockal_env, var_name, var_value))
		return (0);
	if (!is_correct_varname(var_name))
		return (printf("create new envv: not a valid name %s", var_name), NULL);
	new_node = create_new_node(var_name, var_value);
	if (!new_node)
		return (1);
	add_node_to_end(lockal_env, new_node);
	return (0);
}
