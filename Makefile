NAME    = libezflags.a
CC      = cc
CFLAGS  = -Werror -I include/ -I src/ -std=c99

SRCS    = $(shell find src -name "*.c")

OBJS    = $(patsubst src/%.c, .build/%.o, $(SRCS))

TEST_SRCS   = $(wildcard tests/*.c)
TEST_BIN    = .build/ezflags_test

GREEN   = \033[0;32m
CYAN    = \033[0;36m
RESET   = \033[0m

DEBUG_CFLAG = -fsanitize=undefined,address -Wall -Wextra -Wformat=2 -Wformat-signedness -Wwrite-strings -Wcast-qual -Wformat-overflow=2 -Wstringop-overflow -Wconversion -Wswitch-default -Wimplicit-fallthrough=4 -g

all: $(NAME)

debug: CFLAGS += $(DEBUG_CFLAG)
debug: clean test

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@printf "  AR  ->  $(NAME)\n"
	@printf "$(GREEN)$(NAME) ready$(RESET)\n"

.build/%.o: src/%.c
	@mkdir -p $(dir $@)
	@printf "  CC  ->  $<\n"
	@$(CC) $(CFLAGS) -c $< -o $@

test: $(NAME)
	@printf "$(CYAN)-- tests ----------------------$(RESET)\n"
	@mkdir -p .build
	@$(CC) $(CFLAGS) $(TEST_SRCS) -L. -lezflags -o $(TEST_BIN)
	@printf "  BIN ->  $(TEST_BIN)\n"
	@printf "$(CYAN)-- running --------------------$(RESET)\n"
	@./$(TEST_BIN)

clean: clean
	@rm -rf .build
	@printf "  RM  ->  .build/\n"
	@rm -f $(NAME)
	@printf "  RM  ->  $(NAME)\n"

re: clean all

.PHONY: all clean fclean re test
