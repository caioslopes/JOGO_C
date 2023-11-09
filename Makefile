##
## C/SDL2 Raycaster Makefile
##

SRC		=	./src/main.c	\
			./src/color.c	\
			./src/map.c		\
			./src/buttons.c \
			./src/player.c  \
			./src/monter.c  \
			./src/queue.c   

NAME	=	raycaster

CC		=	gcc

RM		=	rm -f

OBJ		=	$(SRC:.c=.o)

CFLAGS	=	-O2 -W -Wall -Wextra -Werror
CFLAGS	+=	-I./lib/

LDLIBS	=	-lSDL2 -lm

all		:	$(NAME)

$(NAME)	:	$(OBJ)
			$(CC) -o $(NAME) $(OBJ) $(LDLIBS)

clean	:
			$(RM) $(OBJ)

fclean	:	clean
			$(RM) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re
