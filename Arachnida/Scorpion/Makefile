NAME	=	scorpion
OBJS	=	$(SRCS:.cpp=.o)
CC		=	c++
FLAGS	=	-Wall -Wextra -Werror -std=c++98
SRCS	=	Scorpion.cpp	\
			Main.cpp		\

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(FLAGS) $(SRCS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONNY: all clean fclean re