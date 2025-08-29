/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devriez <devriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:00:01 by devriez           #+#    #+#             */
/*   Updated: 2025/08/28 19:11:29 by devriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_redirect_type {
	INPUT,	   // <
	OUTPUT,	  // >
	APPEND,	  // >>
	HEREDOC	  // <<
} t_redirect_type;

typedef struct s_redirect {
	t_redirect_type		type;
	char				*file;
	struct s_redirect	*next;
} t_redirect;

typedef struct s_command {
	char				**args;
	t_redirect			*redirections; // Eine Liste von Umleitungen
	struct s_command	*next;		// Ein Zeiger auf den nächsten Befehl (für Pipes)
} t_command;

typedef struct s_env {
	char			*name;
	char			*value;
	struct s_env	*next;
} t_env;

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
bool	is_internal(char *cmd);
int		execute_internal(t_command *cmd, t_env *lockal_envp);

//get_cmd_path.c
char	*get_cmd_path(char *cmd_name, char **envp);

//buildins
int	echo_buildtin(t_command *cmd);
int	cd_builtin(t_command *cmd, t_env **lockal_envp);

//env
char	*get_env_var(t_env *lockal_envp, char *var_name);
int		set_env_var(t_env **lockal_env, char *var_name, char *var_new_value);
t_env	*env_to_list(char **envp);

//error_and_free
void	ft_free_env_list(t_env *head);

//delete !!!!!!11
t_command	*johannes_func(char	*line);




#endif