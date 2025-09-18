/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:00:01 by devriez           #+#    #+#             */
/*   Updated: 2025/09/18 16:24:35 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h> // size_t
# include <stdbool.h> // type bool

typedef enum e_redirect_type {
	INPUT,	   // <
	OUTPUT,	  // >
	APPEND,	  // >>
	HEREDOC,	  // <<
} t_redirect_type;

typedef enum e_word_type {
	W_INPUT,	   // <
	W_OUTPUT,	  // >
	W_APPEND,	  // >>
	W_HEREDOC,	  // <<
	W_PIPE,  // |
	W_WORD 
} t_word_type;

typedef enum e_word_flags {
	Q_NONE = 0,
	Q_SQ = 1 << 0,
	Q_DQ = 1 << 1,
	Q_ESC = 1 << 2,
	Q_EXPD = 1 << 3
}	t_word_flags;

typedef struct	s_redirect {
	t_redirect_type		type;
	char				*file;
	int					heredoc_fd;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command {
	char				*name;
	char				**args;
	t_redirect			*redirections;
	struct s_command	*next;
}	t_command;

typedef struct s_word {
	char			*text;
	t_word_flags	*mask;
	size_t			len;
}	t_word;

typedef struct s_meta
{
	unsigned has_qoutes : 1;
	unsigned is_here_delim : 1;
	unsigned delim_qouted : 1;
}	t_meta;

typedef struct s_tokens {
	char			*text;
	t_word_flags	*mask;
	size_t			len;
	t_word_type		type;
	t_meta			meta;
}	t_tokens;

typedef struct s_mini
{
 struct t_env *env;
 struct t_hist *hist;
 int    last_exit_status[1];
} t_mini;

typedef struct s_hist {
 char   *prompt;
 struct s_hist *before;
 struct s_hist *next;
} t_hist;

typedef struct s_env
{
 char   *name;
 char   *value;
 struct s_env *next;
} t_env;

# include "libft.h" // libft
# include <unistd.h> // dup2, fork, chdir, open, close
# include <sys/types.h> // fork(pid_t)
# include <stdio.h> // printf
# include <stdlib.h> // exit
# include <signal.h> // signal
# include <readline/readline.h> // readline
# include <readline/history.h> // add_history
# include <fcntl.h> // open() - flags
#include <wait.h>

extern int	g_last_exit_status;
extern int	g_should_exit;

//execution
// child_process.c
void	child_setup(int fd_in, int fd_out);
int		child_process(t_command *cmd, t_env *local_env);
//error_and_free
void	free_arr(char **arr);
void	free_env_list(t_env *head);
void	free_command(t_command *head);
//	execution.c
void	handle_single_cmd(t_command *cmd, t_env *local_env);
void	handle_multiply_cmds(t_command *cmd, t_env *local_env);
//	get_cmd_path.c
char	*get_cmd_path(char *cmd_name, t_env *local_env);
//handle_redirect.c
int		handle_redirect(t_redirect *redirectoins);
int		redirect_in_parent(t_redirect *redir,
	int *saved_stdin,
	int *saved_stdout);
void	restore_parent_io(int saved_stdin, int saved_stdout);
//utils.c
void	handle_signal(int signum);
char	**join_str_with_arr(char *str, char **arr);

//buildins
int		cd_builtin(t_command *cmd, t_env *local_env);
int		echo_builtin(t_command *cmd, t_env *env);
int		env_builtin(t_command *cmd, t_env *local_env);
int		execute_builtin(t_command *cmd, t_env *local_env);
bool	is_builtin(char *cmd);
int		exit_builtin(t_command *cmd);
int		export_builtin(t_command *cmd, t_env *local_env);
int		pwd_builtin(t_env *local_env);
int		unset_builtin(t_command *cmd, t_env **local_env);

//env
// convert_env
t_env	*env_to_list(char **envp);
char	**env_to_char_array(t_env *env_list);
//	env_utils
bool	is_correct_varname(char *name);
bool	is_env_var_exist(t_env *local_env, char *var_name);
char	**parse_envv(char *name_eq_value);
void	sort_env(t_env *local_env);
void	free_node(t_env *node);
//	get_and_del
char	*get_env_var_val(t_env *local_env, char *var_name);
void	remove_envv(t_env **local_env, char *var_name);
//	set_envv
int		set_envv_from_pair(t_env **local_env, char *name, char *value);
int		set_envv_from_str(t_env **local_env, char *name_equal_value);

//parser
t_command	*parse_command_line(char *line, t_env *env);
char		**lex(char const *s);
t_tokens	*get_type(char **lexed, t_tokens **tokens);

#endif