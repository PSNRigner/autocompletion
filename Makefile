##
## Makefile for  in /home/frasse_l/Projets/autocompletion
## 
## Made by loic frasse-mathon
## Login   <frasse_l@epitech.net>
## 
## Started on  Thu Jun 30 09:03:28 2016 loic frasse-mathon
## Last update Thu Jun 30 09:50:04 2016 loic frasse-mathon
##

NAME		= autocompletion

SRC		= src/main.cpp \
		src/autocompletion.cpp

OBJ		= $(SRC:.cpp=.o)

CPP		= g++ -g

RM		= rm -f

CPPFLAGS	= -I ./include -O3 -W -Wall -Werror -Wextra

all:		$(NAME)

$(NAME):	$(OBJ)
		$(CPP) -o $(NAME) $(OBJ)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
