NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c utils.c ft_atoi.c philo_actions.c philo_init.c philo_states.c philo_threads.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
