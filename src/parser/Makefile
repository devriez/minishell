CC       := gcc
CFLAGS   := -std=c11 -Wall -Wextra -Werror
SAN      := -fsanitize=address,undefined
CPPFLAGS := -I. -I../includes -I../../libft

SRC       := parser.c
TEST_SRC  := tests/test_parser.c

OBJ_DIR   := obj
OBJS      := $(SRC:%.c=$(OBJ_DIR)/%.o)
TEST_OBJS := $(TEST_SRC:%.c=$(OBJ_DIR)/%.o)
DEPS      := $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)
BIN       := test

# Libft
LIBFT_DIR := ../../libft
LIBFT     := $(LIBFT_DIR)/libft.a

.PHONY: all run clean fclean re

all: $(BIN)

$(BIN): $(LIBFT) $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(SAN) $(OBJS) $(TEST_OBJS) $(LIBFT) -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Generic compile rule: turn foo.c into obj/foo.o
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(BIN)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEPS)
