/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_and_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/28 18:59:28 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*create_env_node(char *env_var)
{
	t_env	*new_node;
	char	*equal_sign;

	new_node = malloc(sizeof(*new_node));
	if (!new_node)
		return (NULL);
	equal_sign = ft_strchr(env_var, '=');
	if (!equal_sign)
	{
		free(new_node);
		return (NULL);
	}
	new_node->name = ft_strndup(env_var, equal_sign - env_var);
	new_node->value = ft_strdup(equal_sign + 1);
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

t_env	*env_to_list(char **envp)
{
	int		i;
	t_env	*head;
	t_env	*current;
	t_env	*new_node;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (!new_node)
			return (NULL);
		if (!head)
		{
			head = new_node;
			current = head;
		}
		else
		{
			current->next = new_node;
			current = new_node;
		}
		i++;
	}
	return (head);
}

char	*get_env_var(t_env *lockal_env, char *var_name)
{
	int	i;
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

int add_node_to_end(t_env **head, char *name, char *value)
{
	t_env *new_node;
	t_env *last;

	new_node = create_new_node(name, value);
	if (!new_node)
		return (1);
	if (*head == NULL)
	{
		*head = new_node;
		return (0);
	}
	last = *head;
	while (last->next != NULL)
		last = last->next;
	last->next = new_node;
	return (0);
}

int	set_env_var(t_env **lockal_env, char *var_name, char *var_new_value)
{
	t_env	*current;
	t_env	*last_node;
	char	*new_env_var;
	char	*temp;

	current = *lockal_env;
	while (current)
	{
		if (ft_strcmp(current->name, var_name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(var_new_value);
			if (!current->value)
				return (1);
			return (0);
		}
		current = current->next;
	}
	return (add_node_to_end(lockal_env, var_name, var_new_value));
}
