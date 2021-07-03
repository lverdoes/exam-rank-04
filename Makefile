# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: lverdoes <lverdoes@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2021/07/03 11:09:25 by lverdoes      #+#    #+#                  #
#    Updated: 2021/07/03 11:10:47 by lverdoes      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

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
