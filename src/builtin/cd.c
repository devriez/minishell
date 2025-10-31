/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:00:01 by devriez           #+#    #+#             */
/*   Updated: 2025/10/30 14:06:18 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_pwd_and_oldpwd(t_env **local_env)
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
	if (set_envv_from_pair(local_env, "PWD", getcwd(NULL, 0)) == 1)
	{
		printf("cd: error in set PWD\n");
		return (1);
	}
	free(pwd_val);
	return (0);
}

static char	*cd_error_handler(char *input, t_env *local_env)
{
	char	*path;

	path = NULL;
	if (!input || ft_strcmp(input, "~") == 0 || ft_strcmp(input, "..") == 0)
	{
		path = ft_strdup(get_env_var_val(local_env, "HOME"));
		if (!path)
			return (printf("cd: HOME not set\n"), NULL);
	}
	if (ft_strcmp(input, "..") == 0)
	{
		path = ft_strdup(".."); 
		if (!path)
			return (NULL);
	}
	else if (ft_strcmp(input, "-") == 0)
	{
		path = ft_strdup(get_env_var_val(local_env, "OLDPWD"));
		if (!path)
			return (printf("cd: OLDPWD not set\n"), NULL);
		printf("%s\n", path);
	}
	return (path);
}

static char	*get_cd_path(t_command *cmd, t_env *local_env)
{
	char	*input;
	char	*path;

	input = NULL;
	if (cmd->args)
	{
		if (cmd->args[1])
			perror("cd");
		if (cmd->args[1])
			return (NULL);
		input = cmd->args[0];
	}
	path = cd_error_handler(input, local_env);
	if (path)
		return (path);
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
	{
		if (errno == ENOENT)
			printf("minishell: cd: %s: No such file or directory\n", path);
		else if (errno == EACCES)
			printf("minishell: cd: %s: Permission denied\n", path);
		else if (errno == ENOTDIR)
			printf("minishell: cd: %s: Not a directory\n", path);
		else
			perror("cd");
		free(path);
		return (1);
	}
	if (update_pwd_and_oldpwd(&local_env) == 1)
		return (1);
	free(path);
	return (0);
}
