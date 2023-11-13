##
## C/SDL2 Raycaster Makefile
##

SRC		=	./src/raycaster.c	\
			./src/color.c	\
			./src/map.c		\
			./src/buttons.c \
			./src/player.c  \
			./src/monster.c  \
			./src/queue.c   

NAME	=	game_exec

CC		=	gcc

RM		=	rm -f

OBJ		=	$(SRC:.c=.o)

CFLAGS	=	-O2 -W -Wall -Wextra -Werror
CFLAGS	+=	-I./lib/

LDLIBS	=	-lSDL2 -lSDL2_mixer -lm

all		:	$(NAME)

$(NAME)	:	$(OBJ)
			$(CC) -o $(NAME) $(OBJ) $(LDLIBS)

clean	:
			$(RM) $(OBJ)

fclean	:	clean
			$(RM) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re
