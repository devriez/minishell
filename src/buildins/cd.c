/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:00:01 by devriez           #+#    #+#             */
/*   Updated: 2025/08/28 19:03:40 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void update_cd_env(t_env **lockal_envp, char *old_pwd_val, char *path)
{
	set_env_var(lockal_envp, "OLDPWD", old_pwd_val);
	set_env_var(lockal_envp, "PWD", path);
}

char *get_cd_path(char *arg, t_env *lockal_envp)
{
	char *path;

	if (!arg || ft_strcmp(arg, "~") == 0)
	{
		path = get_env_var(lockal_envp, "HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (ft_strcmp(arg, "-") == 0)
	{
		path = get_env_var(lockal_envp, "OLDPWD");
		if (!path)
		{
			printf("minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		printf("%s\n", path);
	}
	else
		path = ft_strdup(arg);
	return (path);
}

int cd_builtin(t_command *cmd, t_env **lockal_envp)
{
	char	*path;
	char	*old_pwd_val;

	path = get_cd_path(cmd->args[1], *lockal_envp);
	if (!path)
		return (1);
	if (chdir(path) == -1)
		return (free(path), perror("minishell: cd"), 1);
	old_pwd_val = get_env_var(*lockal_envp, "PWD");
	if (!old_pwd_val)
		return (printf("minishell: cd: PWD not set\n"), 1);
	update_cd_env(lockal_envp, old_pwd_val, path);
	free(path);
	free(old_pwd_val);
	return (0);
}
