/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoiseik <amoiseik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:00:01 by devriez           #+#    #+#             */
/*   Updated: 2025/10/30 18:00:30 by amoiseik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h" // libft
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
# include <errno.h> // errno ENOENT EACCES ENOTDIR
# include <sys/stat.h> //stat S_ISDIR

extern volatile sig_atomic_t	g_last_signal;

typedef enum e_redirect_type 
{
	INPUT,
	OUTPUT,
	APPEND,
	HEREDOC
}	t_redirect_type;

typedef enum e_word_type {
	W_INPUT,
	W_OUTPUT,
	W_APPEND,
	W_HEREDOC,
	W_PIPE,
	W_WORD,
	W_NONE
}	t_word_type;

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

typedef enum e_word_flags {
	Q_NONE = 0,
	Q_SQ = 1 << 0,
	Q_DQ = 1 << 1,
	Q_ESC = 1 << 2,
	Q_EXPD = 1 << 3
}	t_word_flags;

typedef struct s_wordm
{
	char			*s;
	t_word_flags	*mask;
}	t_wordm;

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
	t_env	*env;
	int		last_exit_status;
	char	*shell_name;
	pid_t	pids[256];
	int		pid_count;
}	t_mini;

typedef struct s_lex_help
{
	char	*s;
	size_t	span;
}	t_lex_help;

char			*extract_var_name(t_wordm str, size_t start, size_t *len);
int				wait_exit_statuses(t_mini *mini);
void			close_all_other_pipes(int fd_in, int fd_out);
int				execute_helper(t_command *cmd, t_mini *mini, \
										int fd_in, int *pipe_fd);
int				fork_and_execute(t_command *cmd, \
								t_mini *mini, \
								int fd_in, \
								int fd_out);
int				child_setup(t_command *cmd, int fd_in, int fd_out);
int				child_process(t_command *cmd, t_env *local_env);
void			free_arr(char **arr);
void			free_env_list(t_env *head);
void			free_command(t_command *head);
void			handle_single_cmd(t_command *cmd, t_mini *mini);
void			handle_multiply_cmds(t_command *cmd, t_mini *mini);
char			*get_cmd_path(char *cmd_name, t_env *local_env);
int				handle_redirect(t_redirect *redirectoins);
int				redirect_in_parent(t_redirect *redir, \
									int *saved_stdin, \
									int *saved_stdout);
void			restore_parent_io(int saved_stdin, int saved_stdout);
void			set_sigint(int signum);
void			ignore_sigint(int signum);
void			ignore_sigint_print_newline(int signum);
void			signal_handler_wait(int signum);
void			setup_signals_to_default(void);
void			setup_signals(void (*signal_handler)(int));
char			**join_str_with_arr(char *str, char **arr);
int				cd_builtin(t_command *cmd, t_env *local_env);
int				echo_builtin(t_command *cmd);
int				env_builtin(t_command *cmd, t_env *local_env);
int				execute_builtin(t_command *cmd, t_env *local_env);
bool			is_builtin(char *cmd);
int				exit_builtin(t_command *cmd);
int				export_builtin(t_command *cmd, t_env *local_env);
int				pwd_builtin(t_env *local_env);
int				unset_builtin(t_command *cmd, t_env **local_env);
t_env			*env_to_list(char **envp);
char			**env_to_char_array(t_env *env_list);
bool			is_correct_varname(char *name);
bool			is_env_var_exist(t_env *local_env, char *var_name);
char			**parse_envv(char *name_eq_value);
void			sort_env(t_env *local_env);
void			free_node(t_env *node);
char			*get_env_var_val(t_env *local_env, char *var_name);
void			remove_envv(t_env **local_env, char *var_name);
int				set_envv_from_pair(t_env **local_env, char *name, char *value);
int				set_envv_from_str(t_env **local_env, char *name_equal_value);
t_command		*parse_tokens_to_commands(t_tokens *tokens, \
											size_t count, t_env *env);
t_tokens		*lex(char *s);
t_command		*parse_command_line(char *line, t_mini mini);
t_redirect		read_heredoc(char *delim, int type);
t_tokens		*get_type(t_tokens *tokens);
int				expand_tokens(t_tokens *tokens, size_t token_count, \
					t_mini mini);
char			*expand_token_text(char *text, t_word_flags *mask, t_mini mini);
char			*ft_strjoin_char(char *str, char c);
char			*expand_single_var(t_wordm str, size_t start, t_mini mini, \
										size_t *new_len);
int				should_expand_in_context(t_word_flags *mask, size_t pos);
void			expand_alt(size_t *i, char **result, char *original);
int				is_parent(char c);
int				op_len(const char *s);
int				count_words(const char *s);
int				get_word_span(const char *s, size_t *emit, size_t *span);
void			lex_free(t_tokens *arr, int i);
void			init_state(t_meta *meta, size_t *i, size_t *j, char *in_parent);
int				handle_quote(char c, char *in_parent, t_meta *meta);
void			copy_and_mark(char c[2], char *out, \
					t_word_flags *mask, size_t *j);
size_t			*find_pipe_positions(t_tokens *tokens, size_t count, \
					size_t *pipe_count);
t_redirect_type	convert_word_type_to_redirect(t_word_type type);
t_redirect		*parse_redirections(t_tokens *tokens, size_t start, size_t end);
void			add_redirect_to_list(t_redirect **head, t_redirect *new_node);
t_redirect		*create_redirect_node(t_redirect_type type, char *file, \
					int fd);
char			**build_args_only_array(t_tokens *tokens, \
					size_t start, size_t end);
size_t			ptr_count(char **v);
char			**build_args_array(t_tokens *tokens, size_t start, size_t end);
char			**allocate_args_array(size_t word_count);
int				is_redirection_target(t_tokens *tokens, size_t pos);
size_t			count_word_tokens(t_tokens *tokens, size_t start, size_t end);
void			free_args_array(char **args);
t_command		*parse_single_command(t_tokens *tokens, \
					size_t start, size_t end);
void			update_shlvl(t_env	*local_env);
int				check_is_directory(const char *cmd_path, const char *cmd_name);
int				cmd_error_not_found(const char *cmd);
void			put_error(const char *cmd_name, const char *msg);
char			*pid_to_array(pid_t n);

#endif