/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:26 by devriez           #+#    #+#             */
/*   Updated: 2025/08/29 16:17:18 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_builtin(t_env *lockal_env)
{
	char	*working_dir;

	working_dir = get_env_var(lockal_env, "PWD");
	if (!working_dir)
	{
		printf("minishell: pwd: PWD not set\n");
		return (1);
	}
	printf("%s\n", working_dir);
	free(working_dir);
	return (0);
}
