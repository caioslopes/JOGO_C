#include "../lib/game.h"

int main(){
    Game game;

    init_game(&game);
    game_running(game);
    quit_aplication(&game);
}