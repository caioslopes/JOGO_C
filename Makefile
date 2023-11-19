#Makefile compiler

SOURCE = 	./src/all.c     \
			./src/buttons.c \
			./src/fonts.c   \
			./src/game.c    \
			./src/main.c    \
			./src/map.c     \
			./src/monster.c \
			./src/player.c  \
			./src/queue.c   \
			./src/sounds.c  \
			./src/sprites.c
			

SRC = ./src/*.c

NAME = exec

CC = gcc

RM = rm -f

OBJ = $(SOURCE:.c=.o)

#Flags and Library on Linux
CFLAGS	=	-O2 -W -Wall -Wextra -Werror
CFLAGS	+=	-I./lib/

LDLIBS	=	-lSDL2 -lSDL2_mixer -lSDL2_ttf -lm

#Folder localy
LIBRARY_PATH	= 	-I./lib

#Library Path on macOs (Apple Silicon - Sonoma)
LIBRARY_PATH	+=  -I/Library/Frameworks/SDL2.framework/Headers
LIBRARY_PATH	+=  -F/Library/Frameworks -framework SDL2
LIBRARY_PATH 	+= 	-I/Library/Frameworks/SDL2_mixer.framework/Headers
LIBRARY_PATH 	+= 	-F/Library/Frameworks -framework SDL2_mixer
LIBRARY_PATH 	+= 	-I/Library/Frameworks/SDL2_ttf.framework/Headers
LIBRARY_PATH 	+= 	-F/Library/Frameworks -framework SDL2_ttf

RPATH 	= 	-rpath /Library/Frameworks

macos	: 	$(SRC)
			$(CC) $(SRC) $(LIBRARY_PATH) -o $(NAME) $(RPATH)

linux	: 	$(OBJ)
			$(CC) -o $(NAME) $(OBJ) $(LDLIBS)

clean	:
			$(RM) $(OBJ)