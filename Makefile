## Variables for Linux Mint
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

LDLIBS	=	-lSDL2 -lSDL2_mixer -lSDL2_ttf -lm

## Variables for macOs
OBJECT 	=  ./src/*.c

CLANG	= 	clang

LIBRARY_PATH	= 	-I./lib
LIBRARY_PATH	+=  -I/Library/Frameworks/SDL2.framework/Headers
LIBRARY_PATH	+=  -F/Library/Frameworks -framework SDL2
LIBRARY_PATH 	+= 	-I/Library/Frameworks/SDL2_mixer.framework/Headers
LIBRARY_PATH 	+= 	-F/Library/Frameworks -framework SDL2_mixer

RPATH 	= 	-rpath /Library/Frameworks

macos	: 	$(OBJECT)
			$(CLANG) $(OBJECT) $(LIBRARY_PATH) -o $(NAME) $(RPATH)

all		:	$(NAME)

$(NAME)	:	$(OBJ)
			$(CC) -o $(NAME) $(OBJ) $(LDLIBS)

clean	:
			$(RM) $(OBJ)

fclean	:	clean
			$(RM) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re
