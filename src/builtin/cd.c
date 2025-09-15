/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:00:01 by devriez           #+#    #+#             */
/*   Updated: 2025/09/11 21:36:31 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_pwd_and_oldpwd(t_env **local_env, char *path)
{
	char	*pwd_val;

	pwd_val = ft_strdup(get_env_var_val(*local_env, "PWD"));
	if (!pwd_val)
	{
		printf("cd: PWD not set\n");
		return (1);
	}
	if (set_envv_from_pair(local_env, "OLDPWD", pwd_val) == 1)
	{
		printf("cd: error in set OLDPWD\n");
		return (1);
	}
	if (set_envv_from_pair(local_env, "PWD", path) == 1)
	{
		printf("cd: error in set PWD\n");
		return (1);
	}
	free(pwd_val);
	return (0);
}

static char	*get_cd_path(t_command *cmd, t_env *local_env)
{
	char	*input;
	char	*path;

	input = NULL;
	if (cmd->args)
		input = cmd->args[1];  // First argument, not command name
	if (!input || ft_strcmp(input, "~") == 0)
	{
		path = ft_strdup(get_env_var_val(local_env, "HOME"));
		if (!path)
			return (printf("cd: HOME not set\n"), NULL);
	}
	else if (ft_strcmp(input, "-") == 0)
	{
		path = ft_strdup(get_env_var_val(local_env, "OLDPWD"));
		if (!path)
			return (printf("cd: OLDPWD not set\n"), NULL);
		printf("%s\n", path);
	}
	else
		path = ft_strdup(input);
	return (path);
}

int	cd_builtin(t_command *cmd, t_env *local_env)
{
	char	*path;

	path = get_cd_path(cmd, local_env);
	if (!path)
		return (1);
	if (chdir(path) == -1)
		return (free(path), perror("minishell: cd:"), 1);
	if (update_pwd_and_oldpwd(&local_env, path) == 1)
		return (1);
	free(path);
	return (0);
}
