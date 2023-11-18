#include "../lib/game.h"

/******************
* APPLICATION
*******************/

struct game{

    // Redering
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *window_surface;
    SDL_Texture *screen_texture;

    // Game information
    bool playing;
    bool menu;
    bool quit;

    //Buttons
    ButtonKeys keys;

};

void init_game(Game *game){
    Game g;
    g = malloc(sizeof(struct game));

    printf("Starting...\n");

    // SDL options
    SDL_Init(SDL_INIT_EVERYTHING);

    g->window = SDL_CreateWindow("Não olhe para trás", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    g->renderer = SDL_CreateRenderer(g->window, -1, SDL_RENDERER_ACCELERATED);

    // Game options
    g->quit = false;
    g->playing = true;
    g->menu = false;

    init_buttons(&g->keys);

    if(game != NULL){
        *game = g;
    }else{
        printf("Error, & is NULL - Game\n");
    }
}

/********************
* RAYCASTER - Engine
*********************/

struct raycaster{
    double player_pos_x;
    double player_pos_y;
    double player_dir_x;
    double player_dir_y;
    double player_plane_x;
    double player_plane_y;
    double ray_dir_x;
    double ray_dir_y;
    int map_x;
    int map_y;
    double side_dist_x;
    double side_dist_y;
    double delta_dist_x;
    double delta_dist_y;
    int step_x;
    int step_y;
    int side;
    int draw_start;
    int draw_end;
    double perp_wall_dist;
};

void init_raycaster(Raycaster *rc){
    Raycaster r;
    r =  malloc(sizeof(struct raycaster));

    // Initial Values
    r->player_pos_x = INIT_P_POS_X;
    r->player_pos_y = INIT_P_POS_Y;
    r->player_dir_x = INIT_P_DIR_X;
    r->player_dir_y = INIT_P_DIR_Y;
    r->player_plane_x = INIT_P_PLANE_X;
    r->player_plane_y = INIT_P_PLANE_Y;

    if(rc != NULL){
        *rc = r;
    }else{
        printf("Error, & is NULL - Raycaster\n");
    }
}

void calculating(Raycaster *rc, int w){
    double camera_x;

    camera_x = 2 * w / (double)(SCREEN_WIDTH)-1;

    (*rc)->ray_dir_x = (*rc)->player_dir_x + (*rc)->player_plane_x * camera_x;
    (*rc)->ray_dir_y = (*rc)->player_dir_y + (*rc)->player_plane_y * camera_x;

    (*rc)->map_x = (int)((*rc)->player_pos_x);
    (*rc)->map_y = (int)((*rc)->player_pos_y);

    (*rc)->delta_dist_x = sqrt(1 + pow((*rc)->ray_dir_y, 2) / pow((*rc)->ray_dir_x, 2));
    (*rc)->delta_dist_y = sqrt(1 + pow((*rc)->ray_dir_x, 2) / pow((*rc)->ray_dir_y, 2));

    if ((*rc)->ray_dir_x < 0){
        (*rc)->step_x = -1;
        (*rc)->side_dist_x = ((*rc)->player_pos_x - (*rc)->map_x) * (*rc)->delta_dist_x;
    }else{
        (*rc)->step_x = 1;
        (*rc)->side_dist_x = ((*rc)->map_x + 1.0 - (*rc)->player_pos_x) * (*rc)->delta_dist_x;
    }

    if ((*rc)->ray_dir_y < 0){
        (*rc)->step_y = -1;
        (*rc)->side_dist_y = ((*rc)->player_pos_y - (*rc)->map_y) * (*rc)->delta_dist_y;
    }else{
        (*rc)->step_y = 1;
        (*rc)->side_dist_y = ((*rc)->map_y + 1.0 - (*rc)->player_pos_y) * (*rc)->delta_dist_y;
    }
}

void dda(Raycaster *rc){
    int hit;
    hit = 0;

    while (hit == 0){
        if ((*rc)->side_dist_x < (*rc)->side_dist_y){
            (*rc)->side_dist_x += (*rc)->delta_dist_x;
            (*rc)->map_x += (*rc)->step_x;
            (*rc)->side = 0;
        }else{
            (*rc)->side_dist_y += (*rc)->delta_dist_y;
            (*rc)->map_y += (*rc)->step_y;
            (*rc)->side = 1;
        }

        if (worldMap[(*rc)->map_x][(*rc)->map_y] > 0){
            hit = 1;
        }
    }
}

int calc_wall_height(Raycaster *rc){
    int line_height;

    if ((*rc)->side == 0){
        (*rc)->perp_wall_dist = ((*rc)->map_x - (*rc)->player_pos_x + (1 - (*rc)->step_x) / 2) / (*rc)->ray_dir_x;
    }else{
        (*rc)->perp_wall_dist = ((*rc)->map_y - (*rc)->player_pos_y + (1 - (*rc)->step_y) / 2) / (*rc)->ray_dir_y;
    }

    line_height = (int)(SCREEN_HEIGHT / (*rc)->perp_wall_dist);
    (*rc)->draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;

    if ((*rc)->draw_start < 0){
        (*rc)->draw_start = 0;
    }

    (*rc)->draw_end = line_height / 2 + SCREEN_HEIGHT / 2;

    if ((*rc)->draw_end >= SCREEN_HEIGHT){
        (*rc)->draw_end = SCREEN_HEIGHT - 1;
    }

    return line_height;
}

void draw_point(Raycaster *rc, int x, SDL_Renderer *renderer){

    int line_height = calc_wall_height(rc);

    //calculate value of wallX
    double wallX; //where exactly the wall was hit

    if ((*rc)->side == 0){
        wallX = (*rc)->player_pos_y + (*rc)->perp_wall_dist * (*rc)->ray_dir_y;
    }else{
        wallX = (*rc)->player_pos_x + (*rc)->perp_wall_dist * (*rc)->ray_dir_x;
    }

    wallX -= floor((wallX));

    //x coordinate on the texture
    int texX = (int)(wallX * (double)(TILE));

    if((*rc)->side == 0 && (*rc)->ray_dir_x > 0){
        texX = TILE - texX - 1;  
    }
    if((*rc)->side == 1 && (*rc)->ray_dir_y < 0){
        texX = TILE - texX - 1;
    }
    
    float ty_step = 31.0/(float)line_height;
    float ty_off = 0;

    if(line_height > SCREEN_HEIGHT){
        ty_off=(line_height-SCREEN_HEIGHT) / 2.0;
        line_height = SCREEN_HEIGHT;
    }

    float ty = ty_off * ty_step;
    ty += 1;

    texX =  (int)((float)texX * (32.0/(float)TILE));
    for(int y = (*rc)->draw_start; y < (*rc)->draw_end; y++){
        
        int pixel = ((int)(ty)*32+texX) * 3 + (32*32*3);

        int r = all[pixel+0];
        int g = all[pixel+1];
        int b = all[pixel+2];

        if((*rc)->side == 1){
            r -= 30;
            g -= 30;
            b -= 30;
        }

        r -= (int)((*rc)->perp_wall_dist * 9);
        g -= (int)((*rc)->perp_wall_dist * 9);
        b -= (int)((*rc)->perp_wall_dist * 9);

        if (r < 0){ r = 0; }
        if (g < 0){ g = 0; }
        if (b < 0){ b = 0; }

        ty += ty_step;

        SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer, x, y);
    }

}

void render_frame(SDL_Renderer *renderer)
{
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

int moviment(Raycaster *rc, ButtonKeys keys){
    double oldDirX;
    double oldPlaneX;

    if (get_w(keys) == 1){
        if (worldMap[(int)((*rc)->player_pos_x + (*rc)->player_dir_x * MV_SPEED)][(int)((*rc)->player_pos_y)] == 0)
            (*rc)->player_pos_x += (*rc)->player_dir_x * MV_SPEED;
        if (worldMap[(int)((*rc)->player_pos_x)][(int)((*rc)->player_pos_y + (*rc)->player_dir_y * MV_SPEED)] == 0)
            (*rc)->player_pos_y += (*rc)->player_dir_y * MV_SPEED;
    }
    if (get_s(keys)){
        if (worldMap[(int)((*rc)->player_pos_x - (*rc)->player_dir_x * MV_SPEED)][(int)((*rc)->player_pos_y)] == 0)
            (*rc)->player_pos_x -= (*rc)->player_dir_x * MV_SPEED;
        if (worldMap[(int)((*rc)->player_pos_x)][(int)((*rc)->player_pos_y - (*rc)->player_dir_y * MV_SPEED)] == 0)
            (*rc)->player_pos_y -= (*rc)->player_dir_y * MV_SPEED;
    }
    if (get_d(keys)){
        oldDirX = (*rc)->player_dir_x;
        (*rc)->player_dir_x = (*rc)->player_dir_x * cos(-ROT_SPEED) - (*rc)->player_dir_y * sin(-ROT_SPEED);
        (*rc)->player_dir_y = oldDirX * sin(-ROT_SPEED) + (*rc)->player_dir_y * cos(-ROT_SPEED);
        oldPlaneX = (*rc)->player_plane_x;
        (*rc)->player_plane_x = (*rc)->player_plane_x * cos(-ROT_SPEED) - (*rc)->player_plane_y * sin(-ROT_SPEED);
        (*rc)->player_plane_y = oldPlaneX * sin(-ROT_SPEED) + (*rc)->player_plane_y * cos(-ROT_SPEED);
    }
    if (get_a(keys)){
        oldDirX = (*rc)->player_dir_x;
        (*rc)->player_dir_x = (*rc)->player_dir_x * cos(ROT_SPEED) - (*rc)->player_dir_y * sin(ROT_SPEED);
        (*rc)->player_dir_y = oldDirX * sin(ROT_SPEED) + (*rc)->player_dir_y * cos(ROT_SPEED);
        oldPlaneX = (*rc)->player_plane_x;
        (*rc)->player_plane_x = (*rc)->player_plane_x * cos(ROT_SPEED) - (*rc)->player_plane_y * sin(ROT_SPEED);
        (*rc)->player_plane_y = oldPlaneX * sin(ROT_SPEED) + (*rc)->player_plane_y * cos(ROT_SPEED);
    }
        
    return (0);
}

void render_loop(Raycaster *rc, Game *game){
    // FPS
    int fps = 60;
    int frameDelay = 1000 / fps;
    Uint32 frameStart;
    int frameTime;
    
    while (!(*game)->quit){

        frameStart = SDL_GetTicks();

        for (int x = 0; x < SCREEN_WIDTH; x++){
            calculating(rc, x);
            dda(rc);
            draw_point(rc, x, (*game)->renderer);
        }

        render_frame((*game)->renderer);

        moviment(rc, (*game)->keys);

        if (read_keys(&(*game)->keys) != 0){
            (*game)->quit = true;
        }

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }

        printf("FPS: %d\n", frameTime);
    }
}