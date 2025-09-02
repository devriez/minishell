/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:50:06 by amoiseik          #+#    #+#             */
/*   Updated: 2025/09/02 18:16:18 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_to_list(char **envp)
{
	int		i;
	t_env	*lockal_env;
	char	**name_value;

	lockal_env = NULL;
	i = 0;
	while (envp[i])
	{
		name_value = parse_envv(envp[i]);
		if (name_value == NULL)
			return (NULL);
		if (set_env_var_from_pair(&lockal_env, name_value[0], name_value[1]))
		{
			ft_free_env_list(lockal_env);
			return (NULL);
		}
		free(name_value[0]);
		free(name_value[1]);
		free(name_value);
		i++;
	}
	return (lockal_env);
}
