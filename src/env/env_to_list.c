/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/30 14:23:27 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*parse_and_create_env_node(char *pair_name_value)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	t_env	*node;

	equal_sign = ft_strchr(pair_name_value, '=');
	if (!equal_sign)
		return (NULL);
	name = ft_strndup(pair_name_value, equal_sign - pair_name_value);
	value = ft_strdup(equal_sign + 1);
	if (!name || !value)
	{
		free(name);
		free(value);
		return (NULL);
	}
	node = create_env_node_from_pair(name, value);
	free(name);
	free(value);
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
		node = parse_and_create_env_node(envp[i]);
		if (!node)
		{
			ft_free_env_list(head);
			return (NULL);
		}
		add_node_to_end(&head, node);
		i++;
	}
	return (head);
}
