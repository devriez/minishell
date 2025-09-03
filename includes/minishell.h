/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:00:01 by devriez           #+#    #+#             */
/*   Updated: 2025/09/03 19:48:50 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_redirect_type {
	INPUT,	   // <
	OUTPUT,	  // >
	APPEND,	  // >>
	HEREDOC	  // <<
}	t_redirect_type;

typedef struct s_redirect {
	t_redirect_type		type;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command {
	char				*name;
	char				**args;
	t_redirect			*redirections; // Eine Liste von Umleitungen
	struct s_command	*next;		// Ein Zeiger auf den nächsten Befehl (für Pipes)
}	t_command;

typedef struct s_env {
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

# include "libft.h" // libft
# include <unistd.h> // dup2, fork, chdir
# include <sys/types.h> // fork(pid_t)
# include <stdio.h> // printf
# include <stdlib.h> // exit
# include <signal.h> // signal
# include <readline/readline.h> // readline
# include <readline/history.h> // add_history
# include <stdbool.h> // type bool

extern int g_last_exit_status;

//utils.c
void	handle_signal(int signum);

//execution
//	get_cmd_path.c
char	*get_cmd_path(char *cmd_name, t_env *lockal_env);

//buildins
int		execute_builtin(t_command *cmd, t_env *lockal_envp);
bool	is_builtin(char *cmd);
int		echo_builtin(t_command *cmd);
int		cd_builtin(t_command *cmd, t_env *lockal_env);
int		pwd_builtin(t_env *lockal_env);
int		export_builtin(t_command *cmd, t_env *lockal_env);

//env
//	env_utils
bool	is_correct_varname(char *name);
bool	is_env_var_exist(t_env *lockal_env, char *var_name);
char	**parse_envv(char *name_eq_value);
//	get_and_sort
char	*get_env_var(t_env *lockal_env, char *var_name);
void	sort_env(t_env *local_env);
//	set_envv
int		set_envv_from_pair(t_env **lockal_env, char *name, char *value);
int		set_envv_from_str(t_env **lockal_env, char *name_equal_value);
//	env_to_list
t_env	*env_to_list(char **envp);



//error_and_free
void	free_env_list(t_env *head);

//delete !!!!!!11
t_command	*johannes_func(char	*line);




#endif