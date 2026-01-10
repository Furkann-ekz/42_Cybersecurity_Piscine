NAME		=	scorpion
CC			=	c++
FLAGS		=	-Wall -Wextra -Werror -std=c++98
CLASS_DIR	=	Classes

SRCS		=	Main.cpp					\
				Scorpion.cpp				\
				$(CLASS_DIR)/AllClass.cpp	\
				$(CLASS_DIR)/Bmp.cpp		\
				$(CLASS_DIR)/Gif.cpp		\
				$(CLASS_DIR)/Jpg.cpp		\
				$(CLASS_DIR)/Png.cpp		\
				$(CLASS_DIR)/Tif.cpp		\
				$(CLASS_DIR)/Webp.cpp		\
				$(CLASS_DIR)/Svg.cpp

GREEN		=	\033[1;32m
CYAN		=	\033[1;36m
RED			=	\033[1;31m
RESET		=	\033[0m

all: $(NAME)

$(NAME): $(SRCS)
	@echo "$(CYAN)Linking objects...$(RESET)"
	@$(CC) $(FLAGS) $(SRCS) -o $(NAME)
	@echo "$(GREEN)Scorpion is ready to sting! 🦂$(RESET)"

fclean:
	@rm -f $(NAME)
	@echo "$(RED)Binary removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re