NAME = minishell

CC = cc
CC_FLAGS = 

SRC_DIR = src/
INCLUDES_DIR = includes/
BUILDINS_DIR = 
READLINE_FLAGS = -L/opt/homebrew/opt/readline/lib -lreadline
READLINE_INCLUDES = /opt/homebrew/opt/readline/include 

SRCS = $(SRC_DIR)main.c \
		$(SRC_DIR)buildins/echo.c \
		$(SRC_DIR)buildins/execute_internal.c \
		$(SRC_DIR)execution/utils.c \
		$(SRC_DIR)env/get_and_set.c


OBJS = $(SRCS:.c=.o)

# Libft files and directories
LIBFT_PATH = ./libft/
LIBFT = ./libft/libft.a

all: {NAME}

{NAME}:$(OBJS) $(LIBFT)
		$(CC) $(OBJS) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)

# Libft rule
$(LIBFT):
	make -C $(LIBFT_PATH)

%.o: %.c
	$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -I $(LIBFT_PATH) -I $(READLINE_INCLUDES) -c $< -o $@

clean:
	make clean -C $(LIBFT_PATH)
	rm -f $(OBJS)

fclean: clean
		make fclean -C $(LIBFT_PATH)
		rm -rf $(NAME)

re: fclean all