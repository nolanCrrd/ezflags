NAME	= libezflags.a
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -I include/

SRCS		= $(wildcard src/*.c)
OBJS		= $(SRCS:src/%.c=.build/%.o)

TEST_SRCS	= $(wildcard tests/*.c)
TEST_BIN	= .build/ezflags_test

GREEN	= \033[0;32m
CYAN	= \033[0;36m
RESET	= \033[0m

all: $(NAME)
	@printf "$(GREEN)$(NAME) ready$(RESET)\n"

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@printf "  AR  ->  $(NAME)\n"

.build/%.o: src/%.c
	@mkdir -p .build
	@printf "  CC  ->  $<\n"
	@$(CC) $(CFLAGS) -c $< -o $@

test: $(NAME)
	@printf "$(CYAN)-- tests ----------------------$(RESET)\n"
	@$(CC) $(CFLAGS) $(TEST_SRCS) -L. -lezflags -o $(TEST_BIN)
	@printf "  BIN ->  $(TEST_BIN)\n"
	@printf "$(CYAN)-- running --------------------$(RESET)\n"
	@./$(TEST_BIN)

clean:
	@rm -rf .build
	@printf "  RM  ->  .build/\n"

fclean: clean
	@rm -f $(NAME)
	@printf "  RM  ->  $(NAME)\n"

re: fclean all

.PHONY: all clean fclean re test
