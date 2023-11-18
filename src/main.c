#include "../lib/game.h"

int main(){
    Game game;
    Raycaster rc;

    init_game(&game);
    init_raycaster(&rc);
    render_loop(&rc, &game);
}