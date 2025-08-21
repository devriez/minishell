CC       := gcc
CFLAGS   := -std=c11 -Wall -Wextra -Werror -g
SAN      := -fsanitize=address,undefined
CPPFLAGS := -I.          # so test sees lex.h

SRC       := lex.c
TEST_SRC  := tests/test_lex.c

OBJ_DIR   := obj
OBJS      := $(SRC:%.c=$(OBJ_DIR)/%.o)
TEST_OBJS := $(TEST_SRC:%.c=$(OBJ_DIR)/%.o)
DEPS      := $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)
BIN       := test

.PHONY: all run clean fclean re

all: $(BIN)

$(BIN): $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(SAN) $^ -o $@

# Generic compile rule: turn foo.c into obj/foo.o
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(BIN)

re: fclean all

-include $(DEPS)