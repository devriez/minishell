/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johartma <johartma@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:00:01 by devriez           #+#    #+#             */
/*   Updated: 2025/09/03 18:07:50 by johartma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_redirect_type {
	INPUT,	   // <
	OUTPUT,	  // >
	APPEND,	  // >>
	HEREDOC,	  // <<
} t_redirect_type;

typedef enum e_word_type {
	INPUT,	   // <
	OUTPUT,	  // >
	APPEND,	  // >>
	HEREDOC,	  // <<
	PIPE,  // |
	WORD 
} t_word_type;

typedef enum e_word_flags {
	Q_NONE = 0,
	Q_SQ = 1 << 0,
	Q_DQ = 1 << 1,
	Q_ESC = 1 << 2,
	Q_EXPD = 1 << 3
}	t_word_flags;

typedef struct s_redirect {
	t_redirect_type		type;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command {
	char				*name;
	char				**args;
	char				*name;
	t_redirect			*redirections; // Eine Liste von Umleitungen
	struct s_command	*next;		// Ein Zeiger auf den nächsten Befehl (für Pipes)
}	t_command;

typedef struct s_env {
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_history {
	char				*prompt;
	struct s_history	*before;
	struct s_history	*next;
}	t_history;

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

//get_cmd_path.c
char	*get_cmd_path(char *cmd_name, char **envp);

//buildins
int		execute_internal(t_command *cmd, t_env *lockal_envp);
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

char	**lex(char const *s);

t_tokens	*get_type(char **lexed, t_tokens **tokens);


#endif