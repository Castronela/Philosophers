CC		= gcc -g
CFLAGS	= -Wall -Wextra -Werror

SRCS	= main.c utils.c
OBJS	= $(SRCS:.c=.o)

NAME	= philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

.o: .c
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -rf $(OBJS) 

fclean: clean
	rm -rf $(NAME)

re: fclean all 
