/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:26 by devriez           #+#    #+#             */
/*   Updated: 2025/09/11 12:57:00 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_builtin(t_env *local_env)
{
	char	*working_dir;

	working_dir = ft_strdup(get_env_var_val(local_env, "PWD"));
	if (!working_dir)
	{
		printf("minishell: pwd: PWD not set\n");
		return (1);
	}
	printf("%s\n", working_dir);
	free(working_dir);
	return (0);
}
