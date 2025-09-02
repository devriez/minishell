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
	char				**args;
	char				*name;
	t_redirect			*redirections; // Eine Liste von Umleitungen
	struct s_command	*next;		// Ein Zeiger auf den nächsten Befehl (für Pipes)
}	t_command;

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

char	**lex(char const *s);

t_tokens	*expand(char **lexed);
t_tokens	*get_type(char **lexed, t_tokens **tokens);
