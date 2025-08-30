/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_and_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:42:50 by amoiseik          #+#    #+#             */
/*   Updated: 2025/08/30 13:55:45 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sys_error(char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void	log_error(char *message)
{
	write(2, message, ft_strlen(message));
	exit(EXIT_FAILURE);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i ++;
	}
	free(arr);
}

void	ft_free_env_list(t_env *head)
{
	t_env	*current;
	t_env	*next_node;

	current = head;
	while (current)
	{
		next_node = current->next;
		if (current->name)
			free(current->name);
		if (current->value)
			free(current->value);
		free(current);
		current = next_node;
	}
}
