#include "../lib/game.h"

int main(int argc, char *argv[]){
    Game game;

    init_game(&game);
    game_running(game);
    quit_aplication(&game);
}