NAME = minishell

CC = cc
CC_FLAGS = -std=c11 -Wall -Wextra -Werror

SRC_DIR = src
INCLUDES_DIR = includes
READLINE_FLAGS = -L/opt/homebrew/opt/readline/lib -lreadline
READLINE_INCLUDES = /opt/homebrew/opt/readline/include 

# Core parsing and execution sources
SRCS = $(SRC_DIR)/main.c \
		$(SRC_DIR)/lex/lex.c \
		$(SRC_DIR)/type/typing/typing.c \
		$(SRC_DIR)/expansion/expansion.c \
		$(SRC_DIR)/heredoc_handler/heredoc_handler.c \
		$(SRC_DIR)/parser/parser.c \
		$(SRC_DIR)/parser/parse_command_line.c \
		$(SRC_DIR)/builtin/cd.c \
		$(SRC_DIR)/builtin/echo.c \
		$(SRC_DIR)/builtin/env.c \
		$(SRC_DIR)/builtin/execute_builtin.c \
		$(SRC_DIR)/builtin/exit.c \
		$(SRC_DIR)/builtin/export.c \
		$(SRC_DIR)/builtin/pwd.c \
		$(SRC_DIR)/builtin/unset.c \
		$(SRC_DIR)/env/env_to_list.c \
		$(SRC_DIR)/env/env_utils.c \
		$(SRC_DIR)/env/set_envv.c \
		$(SRC_DIR)/env/convert_env.c \
		$(SRC_DIR)/env/get_and_del.c \
		$(SRC_DIR)/execution/child_process.c \
		$(SRC_DIR)/execution/error_and_free.c \
		$(SRC_DIR)/execution/execution.c \
		$(SRC_DIR)/execution/get_cmd_path.c \
		$(SRC_DIR)/execution/handle_redirect.c \
		$(SRC_DIR)/execution/utils.c

OBJS = $(SRCS:.c=.o)

# Libft files and directories
LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
		$(CC) $(OBJS) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)

# Libft rule
$(LIBFT):
	make -C $(LIBFT_PATH)

%.o: %.c
	$(CC) $(CC_FLAGS) -I $(INCLUDES_DIR) -I $(LIBFT_PATH) -I $(READLINE_INCLUDES) -c $< -o $@

clean:
	make clean -C $(LIBFT_PATH)
	rm -f $(OBJS)

fclean: clean
	make fclean -C $(LIBFT_PATH)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re"$ser