/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:00:01 by devriez           #+#    #+#             */
/*   Updated: 2025/09/22 16:42:54 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h" // libft
# include <unistd.h> // dup2, fork, chdir, open, close
# include <sys/types.h> // fork(pid_t)
# include <sys/wait.h> // waitpid
# include <stdio.h> // printf
# include <stdlib.h> // exit
# include <signal.h> // signal
# include <readline/readline.h> // readline
# include <readline/history.h> // add_history
# include <stdbool.h> // type bool
# include <fcntl.h> // open() - flags

extern volatile sig_atomic_t	g_last_signal;

typedef enum e_redirect_type 
{
	INPUT,
	OUTPUT,
	APPEND,
	HEREDOC
}	t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type		type;
	char				*file;
	int					heredoc_fd;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char				*name;
	char				**args;
	t_redirect			*redirections;
	struct s_command	*next;
}	t_command;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_mini
{
	t_env	*env;
	int		last_exit_status;
	pid_t	pids[256];
	int		pid_count;
}	t_mini;

//execution
// child_process.c
void		child_setup(int fd_in, int fd_out);
int			child_process(t_command *cmd, t_env *local_env);
//error_and_free
void		free_arr(char **arr);
void		free_env_list(t_env *head);
void		free_command(t_command *head);
//	executoin.c
void		handle_single_cmd(t_command *cmd, t_mini *mini);
void		handle_multiply_cmds(t_command *cmd, t_mini *mini);
//	get_cmd_path.c
char		*get_cmd_path(char *cmd_name, t_env *local_env);
//handle_redirect.c
int			handle_redirect(t_redirect *redirectoins);
int			redirect_in_parent(t_redirect *redir, \
								int *saved_stdin, \
								int *saved_stdout);
void		restore_parent_io(int saved_stdin, int saved_stdout);
//handle_signal.c
void		set_sigint(int signum);
void		ignore_sigint(int signum);
void		ignore_sigint_print_newline(int signum);
void		signal_handler_wait(int signum);
void		setup_signals_to_default(void);
void		setup_signals(void (*signal_handler)(int));
//utils.c
char		**join_str_with_arr(char *str, char **arr);

//buildins
int			cd_builtin(t_command *cmd, t_env *local_env);
int			echo_builtin(t_command *cmd, t_env *env);
int			env_builtin(t_command *cmd, t_env *local_env);
int			execute_builtin(t_command *cmd, t_env *local_env);
bool		is_builtin(char *cmd);
int			exit_builtin(t_command *cmd);
int			export_builtin(t_command *cmd, t_env *local_env);
int			pwd_builtin(t_env *local_env);
int			unset_builtin(t_command *cmd, t_env **local_env);

//env
// convert_env
t_env		*env_to_list(char **envp);
char		**env_to_char_array(t_env *env_list);
//	env_utils
bool		is_correct_varname(char *name);
bool		is_env_var_exist(t_env *local_env, char *var_name);
char		**parse_envv(char *name_eq_value);
void		sort_env(t_env *local_env);
void		free_node(t_env *node);
//	get_and_del
char		*get_env_var_val(t_env *local_env, char *var_name);
void		remove_envv(t_env **local_env, char *var_name);
//	set_envv
int			set_envv_from_pair(t_env **local_env, char *name, char *value);
int			set_envv_from_str(t_env **local_env, char *name_equal_value);

//delete !!!!!!!!
t_command	*johannes_func(char *line);

#endif