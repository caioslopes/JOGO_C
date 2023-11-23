#Makefile compiler

SOURCE = 	./src/buttons.c      \
			./src/fonts.c        \
			./src/game.c         \
			./src/main.c         \
			./src/map.c          \
			./src/monster.c      \
			./src/player.c       \
			./src/queue.c        \
			./src/sounds.c       \
			./assets/textures/textures.c     \
			./assets/textures/home_screen.c  \
			./assets/textures/home_screen2.c \
			./assets/textures/game_over.c    \
			./assets/textures/you_escaped.c  \
			./assets/textures/floor_ceiling.c
			

SRC = ./src/*.c ./assets/textures/*.c

NAME = NaoOlhe

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
			
# Put "mingw32-make windows" in terminal
windows :   	
			gcc -std=c99 ./lib/icon.o -o NaoOlhe.exe ./src/*c ./assets/textures/*c -I ./SDL2/include -L ./SDL2/lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -mwindows

clean	:
			$(RM) $(OBJ)