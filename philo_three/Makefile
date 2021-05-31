NAME =	philo_one

HDR =	philosophers.h

SRCS =	main.c \
		libfunc.c \
		phil_create.c \
		utils.c \
		phil_cycle.c \
		phil_cycle_ctl.c \

OBJS =	$(SRCS:.c=.o)

CC =	gcc

CFL =	-Wall -Wextra -Werror -Wno-unused

INC =	-I.

%.o: %.c
	$(CC) $(INC) $(CFL) -g -c $< -o ${<:.c=.o}

all:	$(NAME)

$(NAME): $(OBJS) $(HDR)
	$(CC) $(CFL) $(INC) $(OBJS) -o $(NAME)

clean:
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re:		fclean
		make

.PHONY: all clean fclean re
