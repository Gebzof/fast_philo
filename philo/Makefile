NAME = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

SRCDIR = srcs

SRCS = srcs/philo.c srcs/actions.c srcs/actions_utils.c srcs/init.c srcs/utils.c srcs/utils2.c srcs/status.c

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

OBJDIR = obj

all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@printf "$(GREEN)Compiling minishell : $(WHITE)$< $(RESET)"
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "\r\033[K"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re