CC		= gcc -g
CFLAGS	= -Wall -Wextra -Werror

SRCS	= main.c utils.c mem_alloc.c input_processing.c mutexes.c threads.c philo.c philo_utils0.c philo_utils1.c
OBJS	= $(SRCS:.c=.o)

NAME	= philo

LEAKS	= #-L../../LeakSanitizer -llsan

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LEAKS) -o $@ $^

.o: .c
	$(CC) $(CFLAGS) $(LEAKS) -c $< 

clean:
	rm -rf $(OBJS) 

fclean: clean
	rm -rf $(NAME)

re: fclean all 
