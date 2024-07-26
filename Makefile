# Compiler and flags
CC		= gcc -g
CFLAGS	= -Wall -Wextra -Werror

# Commands
MKDIR	= mkdir -p
RM		= rm -rf

# Source, object and binary files
SRC		= main.c utils.c mem_alloc.c input_processing.c mutexes.c threads.c philo.c philo_utils0.c philo_utils1.c
SRCDIR	= src

OBJ		:= $(SRC:.c=.o)
OBJDIR	= obj

NAME	= philo
BINDIR	= bin

INCDIR	= include

LEAKS	= #-L../../LeakSanitizer -llsan

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(addprefix $(OBJDIR)/, $(OBJ)) | $(BINDIR)
	$(CC) $(CFLAGS) $(LEAKS) $(addprefix $(OBJDIR)/, $(OBJ)) -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(LEAKS) -I$(INCDIR) -c $< -o $@

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(BINDIR):
	$(MKDIR) $(BINDIR)

clean:
	$(RM) $(addprefix $(OBJDIR)/, $(OBJ))
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(BINDIR)/$(NAME)
	$(RM) $(BINDIR)

re: fclean all 
