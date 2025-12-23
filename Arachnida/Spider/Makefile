NAME	=	Spider
OBJS	=	$(SRCS:.cpp=.o)
FLAGS	=	-Wall -Wextra -Werror -std=c++17
LDFLAGS	=	-lcurl


all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Linking objects...$(RESET)"
	c++ $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "$(GREEN)Spider ready! 🕷️$(RESET)"

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re