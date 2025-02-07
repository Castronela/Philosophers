RED    = \033[31m
GREEN  = \033[32m
YELLOW = \033[33m
BLUE   = \033[34m
MAGENTA= \033[35m
CYAN   = \033[36m
WHITE  = \033[37m
RESET  = \033[0m

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

D_SRC	= src
D_OBJ	= obj
D_HED	= include

VPATH	= $(D_SRC)

SRC		= 	philo.c \
			input_validation.c \
			init_data.c \
			init_philos.c \
			threads.c \
			monitor.c \
			philo_threads_1.c \
			philo_threads_2.c \
			utils.c \
OBJ		:= $(addprefix $(D_OBJ)/, $(SRC:.c=.o))

NAME	= philo

all: $(NAME)


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