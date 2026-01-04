NAME	=	scorpion
OBJS	=	$(SRCS:.cpp=.o)
CC		=	c++
FLAGS	=	-Wall -Wextra -Werror -std=c++98
SRCS	=	./Classes/AllClass.cpp	\
			Main.cpp				\
			./Classes/Jpg.cpp		\
			./Classes/Tif.cpp		\
			./Classes/Bmp.cpp		\

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(FLAGS) $(SRCS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONNY: all clean fclean re