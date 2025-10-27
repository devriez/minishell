/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:29:42 by amoiseik          #+#    #+#             */
/*   Updated: 2025/10/27 17:51:29 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	}
}

/*
char		*get_env_var_val(t_env *local_env, char *var_name);
int			set_envv_from_pair(t_env **local_env, char *var_name, char *var_value);


При запуске minishell: Если $SHLVL не существует в исходном окружении, 
его нужно установить в 1.

При запуске minishell из другой оболочки: Если вы запускаете вашу minishell из, скажем, 
bash (где $SHLVL равен 1), 
ваша minishell должна прочитать это значение, увеличить его на 1 и установить свой $SHLVL в 2.

При запуске minishell: $0 должно быть установлено в то имя, с которым была запущена ваша программа. 
Например, если вы запускаете ее командой ./minishell, $0 должно быть равно ./minishell. 
Если вы запускаете ее командой bash, $0 будет bash.

В вашем примере: $0 сначала был zsh, а когда вы запустили bash, он изменился на bash.
*/