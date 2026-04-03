NAME	= libezflags.a

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -I include/

SRCS	= $(wildcard src/*.c)
OBJS	= $(SRCS:src/%.c=.build/%.o)

TEST_SRCS	= $(wildcard tests/*.c)
TEST_BIN	= .build/esflags_test

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

.build/%.o: src/%.c
	mkdir -p .build
	$(CC) $(CFLAGS) -c $< -o $@

test: $(NAME)
	$(CC) $(CFLAGS) $(TEST_SRCS) -L. -lezflags -o $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf .build

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test
