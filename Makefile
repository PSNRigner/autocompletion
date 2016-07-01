##
## Makefile for  in /home/frasse_l/Projets/autocompletion
## 
## Made by loic frasse-mathon
## Login   <frasse_l@epitech.net>
## 
## Started on  Thu Jun 30 09:03:28 2016 loic frasse-mathon
## Last update Fri Jul  1 09:08:18 2016 loic frasse-mathon
##

NAME		= autoCompletion

SRC		= src/main.cpp \
		src/autocompletion.cpp

OBJ		= $(SRC:.cpp=.o)

CPP		= g++ -g

RM		= rm -f

CPPFLAGS	= -I ./include -W -Wall -Werror -Wextra -g -O3

all:		$(NAME)

$(NAME):	$(OBJ)
		$(CPP) -o $(NAME) $(OBJ)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
