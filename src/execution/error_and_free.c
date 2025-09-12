/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_and_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:42:50 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/11 21:27:16 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
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
*/
void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i ++;
	}
	free(arr);
}

void	free_env_list(t_env *head)
{
	t_env	*current;
	t_env	*next_node;

	if (!head)
		return ;
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

static void	free_redirection(t_redirect *head)
{	
	t_redirect	*current;
	t_redirect	*next;
	
	if (!head)
		return ;
	current = head;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
}

void	free_command(t_command *head)
{
	t_command *current;
	t_command *next;
	
	if (!head)
		return ;
	current = head;
	while (current)
	{
		next = current->next;
		if (current->name)
			free(current->name);
		if (current->args)
			free_arr(current->args);
		if (current->redirections)
			free_redirection(current->redirections);
		free(current);
		current = next;
	}
}
