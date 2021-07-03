NAME	= microshell
SRC		= microshell.c
FLAGS	= -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	gcc $(FLAGS) $(SRC) -o $(NAME)

fclean:
	/bin/rm -f $(NAME)

re: fclean all

test: re
	@./$(NAME) /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
