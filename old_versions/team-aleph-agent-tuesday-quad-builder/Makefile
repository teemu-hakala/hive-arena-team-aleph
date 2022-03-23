CC=gcc
CFLAGS=-Wall -Wextra -Werror -O3 -flto -g
CPPFLAGS=-I../src
LDFLAGS=-flto -L../bin -lagent

OBJS=main.o grid.o scouts.o bees_helper.o forager_new.o bees.o attacker.o \
	builders.o
NAME=agent

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
