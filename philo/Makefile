RED    = \033[31m
GREEN  = \033[32m
YELLOW = \033[33m
BLUE   = \033[34m
MAGENTA= \033[35m
CYAN   = \033[36m
WHITE  = \033[37m
RESET  = \033[0m

CC		= gcc -g
CFLAGS	= -Wall -Wextra -Werror

D_SRC	= src
D_OBJ	= obj
D_HED	= include

VPATH	= $(D_SRC)

SRC		= 	philo.c \
			input_validation.c \
			alloc_data.c \
			init_data.c \
			threads.c \
			threads_monitor.c \
			philo_threads_1.c \
			philo_threads_2.c \
			forks_action.c \
			time.c \
			free.c \
			test_fn.c
OBJ		:= $(addprefix $(D_OBJ)/, $(SRC:.c=.o))

NAME	= philo

all: $(NAME)

test: $(NAME) ../tester
	cd ../tester && ./test.sh ../philo/$(NAME)
# awk '{if (NR == 1){prev = $1}; if (prev > $1) {print(prev "\n" $1 "\n")}; prev = $1}' result.txt 
valgrind: all
	valgrind -s --leak-check=full ./$(NAME) 3 1800 140 200 5

helgrind: all
	valgrind --tool=helgrind ./$(NAME) 3 1800 140 200 5

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "$(GREEN)✔  Binary '$(NAME)' ready$(RESET)"

$(D_OBJ)/%.o: %.c $(D_HED)/philo.h | $(D_OBJ)
	@$(CC) $(CFLAGS) -I$(D_HED) -c $< -o $@

$(D_OBJ): 
	@mkdir -p $@

clean: 
	@rm -rf $(D_OBJ)
	@echo "$(YELLOW)🧽 Removed objs$(RESET)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(YELLOW)🧽 Removed binary$(RESET)"

re: fclean all