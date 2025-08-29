/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/29 21:30:57 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_node(char *env_var, char *name, char *value)
{
	t_env	*node;
	char	*equal_sign;

	if (!is_correct_varname(name))
		return (printf("create new envv: not a valid name %s", name), NULL);
	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	if (env_var)
	{
		equal_sign = ft_strchr(env_var, '=');
		if (!equal_sign)
			return (free(node), NULL);
		node->name = ft_strndup(env_var, equal_sign - env_var);
		node->value = ft_strdup(equal_sign + 1);
	}
	else
	{
		node->name = ft_strdup(name);
		node->value = ft_strdup(value);
	}
	node->next = NULL;
	if (!node->name || !node->value)
		return (free(node->name), free(node->value), free(node), NULL);
	return (node);
}










static t_env	*create_env_node(char *env_var)
{
	t_env	*node;
	char	*equal_sign;

	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	equal_sign = ft_strchr(env_var, '=');
	if (!equal_sign)
	{
		free(node);
		return (NULL);
	}
	node->name = ft_strndup(env_var, equal_sign - env_var);
	node->value = ft_strdup(equal_sign + 1);
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

t_env	*env_to_list(char **envp)
{
	int		i;
	t_env	*head;
	t_env	*current;
	t_env	*node;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		node = create_env_node(envp[i]);
		if (!node)
			return (NULL);
		add_node_to_end(head, node);
		i++;
	}
	return (head);
}
