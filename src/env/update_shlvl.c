/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:28:17 by johartma          #+#    #+#             */
/*   Updated: 2025/10/30 14:59:57 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#include <minishell.h>

void	update_shlvl(t_env	*local_env)
{
	char	*shlvl_value_char;
	char	*new_shlvl_value_char;
	int		shlvl_value_int;

	shlvl_value_char = get_env_var_val(local_env, "SHLVL");
	if (!shlvl_value_char)
		set_envv_from_pair(&local_env, "SHLVL", "1");
	else
	{
		shlvl_value_int = atoi(shlvl_value_char);
		shlvl_value_int ++;
		new_shlvl_value_char = ft_itoa(shlvl_value_int);
		set_envv_from_pair(&local_env, "SHLVL", new_shlvl_value_char);
		free(new_shlvl_value_char);
	}
}
