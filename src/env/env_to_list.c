/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/15 19:35:17 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_to_list(char **envp)
{
	int		i;
	t_env	*lockal_env;

	lockal_env = NULL;
	i = 0;
	while (envp[i])
	{
		set_envv_from_str(&lockal_env, envp[i]);
		i ++;
	}
	return (lockal_env);
}
