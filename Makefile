NAME = minishell

#all: {NAME}

{NAME}:
		cc src/main.c -I/opt/homebrew/opt/readline/include -Iincludes -L/opt/homebrew/opt/readline/lib -lreadline