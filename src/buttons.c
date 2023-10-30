#include "../lib/buttons.h"

struct ButtonKeys {
  int w, a, s, d;
};

void init_keys(keys* key){
    keys k;
    k = malloc(sizeof(key) * 4);
    k->w = 0;
    k->a = 0;
    k->s = 0;
    k->d = 0;

    *key = k;
}

int read_keys(keys key){
    SDL_Event   event;

    while (SDL_PollEvent(&event)){

        if (event.type == SDL_QUIT){
            return (-1);
        }
        if (event.type == SDL_KEYDOWN){

        if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w){
            key->w = 1;
        }

        if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s){
            key->s = 1;
        }

        if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d){
            key->d = 1;
        }


        if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a){
            key->a = 1;
        }

        }

        if (event.type == SDL_KEYUP){

            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w){
                key->w = 0;
            }

            if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s){
                key->s = 0;
            }

            if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d){
                key->d = 0;
            }

            if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a){
                key->a = 0;
            }

        }

    }

    return (0);
}

void move_player(keys key, t_raycaster *rc){
    double oldDirX;
    double oldPlaneX;

    
    if(key->w == 1){

        if(worldMap[(int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED)][(int)(rc->player_pos_y)] == 0){ // verifica se aonde o player está é uma posição de valor 0 (sem parede) no mapa  

            rc->player_pos_x += rc->player_dir_x * MV_SPEED; // incrementa a posição do player no eixo X 

        }

        if(worldMap[(int)(rc->player_pos_x)][(int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)] == 0) { // verifica se aonde o player está é uma posição de valor 0 (sem parede) no mapa

                rc->player_pos_y += rc->player_dir_y * MV_SPEED;  // incrementa a posição do player no eixo Y

        }

    }

    if(key->a == 1){
        oldDirX = rc->player_dir_x;
        rc->player_dir_x = rc->player_dir_x * cos(ROT_SPEED) - rc->player_dir_y * sin(ROT_SPEED);
        rc->player_dir_y = oldDirX * sin(ROT_SPEED) + rc->player_dir_y * cos(ROT_SPEED);
        oldPlaneX = rc->player_plane_x;
        rc->player_plane_x = rc->player_plane_x * cos(ROT_SPEED) - rc->player_plane_y * sin(ROT_SPEED);
        rc->player_plane_y = oldPlaneX * sin(ROT_SPEED) + rc->player_plane_y * cos(ROT_SPEED);
    }

    if(key->s == 1){

        if(worldMap[(int)(rc->player_pos_x - rc->player_dir_x * MV_SPEED)][(int)(rc->player_pos_y)] == 0) {

            rc->player_pos_x -= rc->player_dir_x * MV_SPEED;
            
        }

        if(worldMap[(int)(rc->player_pos_x)][(int)(rc->player_pos_y - rc->player_dir_y * MV_SPEED)] == 0) {
        
            rc->player_pos_y -= rc->player_dir_y * MV_SPEED;

        }
    }

    if(key->d == 1){
        oldDirX = rc->player_dir_x;
        rc->player_dir_x = rc->player_dir_x * cos(-ROT_SPEED) - rc->player_dir_y * sin(-ROT_SPEED);
        rc->player_dir_y = oldDirX * sin(-ROT_SPEED) + rc->player_dir_y * cos(-ROT_SPEED);
        oldPlaneX = rc->player_plane_x;
        rc->player_plane_x = rc->player_plane_x * cos(-ROT_SPEED) - rc->player_plane_y * sin(-ROT_SPEED);
        rc->player_plane_y = oldPlaneX * sin(-ROT_SPEED) + rc->player_plane_y * cos(-ROT_SPEED);
    }
}