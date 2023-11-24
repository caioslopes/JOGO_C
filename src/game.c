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

    //Game screens
    int screen;

    //Sounds
    Mix_Music *sound_track;
    Mix_Chunk *monster_walking;
    Mix_Chunk *openning_door;
    Mix_Chunk *closed_door;
    Mix_Chunk *heavy_breathing;
    Mix_Chunk *pick_up_keys;

    //Fonts
    TTF_Font *font;

    //Data
    Raycaster raycaster;
    ButtonKeys keys;
    Map map;
    Player player;
    Queue queue;
    Element element;
    Monster monster;

};

void init_game(Game *game){
    Game g;
    g = malloc(sizeof(struct game));

    printf("Starting...\n");

    // SDL options
    SDL_Init(SDL_INIT_EVERYTHING);

    g->window = SDL_CreateWindow("Não olhe para trás", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    g->renderer = SDL_CreateRenderer(g->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetWindowMinimumSize(g->window, BUFFER_WIDTH, BUFFER_HEIGHT);
    SDL_RenderSetLogicalSize(g->renderer, BUFFER_WIDTH, BUFFER_HEIGHT);
    SDL_RenderSetIntegerScale(g->renderer, 1);

    // Game options
    g->quit = false;
    g->playing = true;
    g->menu = false;

    // Screens options - 1 = home screen, 2 = playing screen,
    // 3 = won screen, 4 = loss screen, 5 = paused screen, 6 = press enter;
    g->screen = 1;

    //Sounds
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
    g->sound_track = Mix_LoadMUS("assets/sounds/sound_track.mp3");
    g->monster_walking = Mix_LoadWAV("assets/sounds/monster_walking.wav");
    g->closed_door = Mix_LoadWAV("assets/sounds/closed_door.wav");
    g->openning_door = Mix_LoadWAV("assets/sounds/openning_door.wav");
    g->heavy_breathing = Mix_LoadWAV("assets/sounds/heavy_breathing.wav");
    g->pick_up_keys = Mix_LoadWAV("assets/sounds/pick_up_keys.wav");

    //Fonts
    TTF_Init();
    g->font = TTF_OpenFont("assets/fonts/slkscr.ttf", 128);

    //Data
    init_raycaster(&g->raycaster);
    init_buttons(&g->keys);
    init_map(&g->map);
    init_player(&g->player);
    init_queue(&g->queue);
    init_element(&g->element);
    init_monster(&g->monster);

    if(game != NULL){
        *game = g;
    }else{
        printf("Error, & is NULL - Game\n");
    }
}

void game_running(Game game){
    play_music(game->sound_track);
    init_raycaster(&game->raycaster);
    render_loop(&game->raycaster, &game);
}

void change_state(Game *game, int screen){
    (*game)->screen = screen;
}

int get_state(Game *game){
    return (*game)->screen;
}

void restart_game(Game *game){

    close_map(&(*game)->map);
    close_buttons(&(*game)->keys);
    close_player(&(*game)->player);
    close_queue(&(*game)->queue);
    close_element(&(*game)->element);
    close_monster(&(*game)->monster);

    init_raycaster(&(*game)->raycaster);
    init_buttons(&(*game)->keys);
    restart_itens();
    init_map(&(*game)->map);
    init_player(&(*game)->player);
    init_queue(&(*game)->queue);
    init_element(&(*game)->element);
    init_monster(&(*game)->monster);
    change_state(game, 1);
    render_loop(&(*game)->raycaster, game);
}

void quit_aplication(Game *game){
    //Rendering
    SDL_DestroyWindow((*game)->window);
    SDL_DestroyRenderer((*game)->renderer);

    //Sounds
    Mix_FreeMusic((*game)->sound_track);
    Mix_FreeChunk((*game)->monster_walking);
    Mix_FreeChunk((*game)->openning_door);
    Mix_FreeChunk((*game)->closed_door);
    Mix_FreeChunk((*game)->heavy_breathing);
    Mix_FreeChunk((*game)->pick_up_keys);
    Mix_CloseAudio();

    //Fonts
    close_font((*game)->font);

    //Data
    close_map(&(*game)->map);
    close_buttons(&(*game)->keys);
    close_player(&(*game)->player);
    close_queue(&(*game)->queue);
    close_element(&(*game)->element);
    close_monster(&(*game)->monster);

    //General SDL
    SDL_Quit();
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

    camera_x = 2 * w / (double)(BUFFER_WIDTH)-1;

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

void dda(Raycaster *rc, Map *map){
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

        if (on_map(*map,(*rc)->map_x,(*rc)->map_y)){
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

    line_height = (int)(BUFFER_HEIGHT / (*rc)->perp_wall_dist);
    (*rc)->draw_start = -line_height / 2 + BUFFER_HEIGHT / 2;

    if ((*rc)->draw_start < 0){
        (*rc)->draw_start = 0;
    }

    (*rc)->draw_end = line_height / 2 + BUFFER_HEIGHT / 2;

    if ((*rc)->draw_end >= BUFFER_HEIGHT){
        (*rc)->draw_end = BUFFER_HEIGHT - 1;
    }

    return line_height;
}

int choosing_texture(Raycaster *rc, Map map){
    //number of the texture that will be loaded
    int texNum = on_map(map, (*rc)->map_x, (*rc)->map_y);

    //rendering doors
    if((texNum > 11 && texNum < 16) || (texNum == 21 || texNum == 31 || texNum == 41 || texNum == 51)){ texNum = 5; }
    if(texNum == 16){ texNum = 6; }
    if(texNum == 17){ texNum = 7; }
    
    //rendering keys
    if(texNum == 9){ texNum = 8; }
    if(texNum == 10){ texNum = 9; }

    return texNum;
}

void draw_texture(Raycaster *rc, int x, SDL_Renderer *renderer, Map map, int red){

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
    
    float ty_step = 32.0/(float)line_height;
    float ty_off = 0;

    if(line_height > BUFFER_HEIGHT){
        ty_off=(line_height-BUFFER_HEIGHT) / 2.0;
        line_height = BUFFER_HEIGHT;
    }

    int texNum = choosing_texture(rc, map);
    float ty = ty_off * ty_step;

    ty += (texNum - 1) * 32; //choosing textures

    texX =  (int)((float)texX * (32.0/(float)TILE));
    for(int y = (*rc)->draw_start; y < (*rc)->draw_end; y++){
        
        int pixel = ((int)(ty)*32+texX) * 3;
        int r, g, b;

        r = textures[pixel+0];
        g = textures[pixel+1];
        b = textures[pixel+2];

        if((*rc)->side == 1){
            r -= 30;
            g -= 30;
            b -= 30;
        }

        r -= (int)((*rc)->perp_wall_dist * 6);
        g -= (int)((*rc)->perp_wall_dist * 6);
        b -= (int)((*rc)->perp_wall_dist * 6);
        
        r += red;
        g -= red;
        b -= red;

        if(r > 255){r = 255;}

        if (r < 0){ r = 0; }
        if (g < 0){ g = 0; }
        if (b < 0){ b = 0; }

        ty += ty_step;

        SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    //FLOOR CASTING (vertical version, directly after drawing the vertical wall stripe for the current x)
    double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

    //4 different wall directions possible
    if((*rc)->side == 0 && (*rc)->ray_dir_x > 0){
        floorXWall = (*rc)->map_x;
        floorYWall = (*rc)->map_y + wallX;
    }else if((*rc)->side == 0 && (*rc)->ray_dir_x < 0){
        floorXWall = (*rc)->map_x + 1.0;
        floorYWall = (*rc)->map_y + wallX;
    }else if((*rc)->side == 1 && (*rc)->ray_dir_y > 0){
        floorXWall = (*rc)->map_x + wallX;
        floorYWall = (*rc)->map_y;
    }else{
        floorXWall = (*rc)->map_x + wallX;
        floorYWall = (*rc)->map_y + 1.0;
    }

    double distWall, distPlayer, currentDist;

    distWall = (*rc)->perp_wall_dist;
    distPlayer = 0.0;

    if ((*rc)->draw_start < 0) (*rc)->draw_end = BUFFER_HEIGHT; //becomes < 0 when the integer overflows

    //draw the floor from drawEnd to the bottom of the screen
    for(int y = (*rc)->draw_end + 1; y < BUFFER_HEIGHT; y++){
        currentDist = BUFFER_HEIGHT / (2.0 * y - BUFFER_HEIGHT); //you could make a small lookup table for this instead

        double weight = (currentDist - distPlayer) / (distWall - distPlayer);

        double currentFloorX = weight * floorXWall + (1.0 - weight) * (*rc)->player_pos_x;
        double currentFloorY = weight * floorYWall + (1.0 - weight) * (*rc)->player_pos_y;

        int floorTexX, floorTexY;
        floorTexX = (int)(currentFloorX * TILE) % 32;
        floorTexY = (int)(currentFloorY * TILE) % 32;

        int pixel = ((int)(floorTexY)*32+floorTexX) * 3 /* + (32*32*3) */;
        int pixel2 = ((int)(floorTexY)*32+floorTexX) * 3 + (32*32*3);
        int r, g, b;
        int r2, g2, b2;

        //Floor
        r = floor_ceiling[pixel+0];
        g = floor_ceiling[pixel+1];
        b = floor_ceiling[pixel+2];

        r -= (int)(currentDist * 6);
        g -= (int)(currentDist * 6);
        b -= (int)(currentDist * 6);

        r -= 30;
        g -= 30;
        b -= 30;
        
        r += red;
        g -= red;
        b -= red;

        //Ceiling
        r2 = floor_ceiling[pixel2+0];
        g2 = floor_ceiling[pixel2+1];
        b2 = floor_ceiling[pixel2+2];

        r2 -= (int)(currentDist * 6);
        g2 -= (int)(currentDist * 6);
        b2 -= (int)(currentDist * 6);

        r2 -= 30;
        g2 -= 30;
        b2 -= 30;
        
        r2 += red;
        g2 -= red;
        b2 -= red;

        //Converting colors
        if(r > 255){r = 255;} 
        if(r2 > 255){r2 = 255;}

        if (r < 0){ r = 0; }
        if (g < 0){ g = 0; }
        if (b < 0){ b = 0; }

        if(r2 < 0){ r2 = 0; }
        if(g2 < 0){ g2 = 0; }
        if(b2 < 0){ b2 = 0; }
        
        //Floor
        SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer, x, y);

        //Ceil
        SDL_SetRenderDrawColor(renderer, r2, g2, b2, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer, x, BUFFER_HEIGHT - y);
    }

}

void draw_screens(Game *game, int screen_number, double shade, double red_shade){

    int pixel = 0;
    int r, g, b;
    printf("entrou\n");
    
    if(screen_number == 1){
        for(int y = 0; y < BUFFER_HEIGHT; y++){
            for(int x = 0; x < BUFFER_WIDTH; x++){

                //Verifications to apply shading transition just in the background image
                if(home_screen[pixel + 0] == 0){ 
                    r = home_screen2[pixel + 0]*shade;
                }else{
                    r = home_screen[pixel + 0];
                }
                if(home_screen[pixel + 1] == 0){ 
                    g = home_screen2[pixel + 1]*shade;
                }else{
                    g = home_screen[pixel + 1];
                }
                if(home_screen[pixel + 2] == 0){ 
                    b = home_screen2[pixel + 2]*shade;
                }else{
                    b = home_screen[pixel + 2];
                }   

                if(red_shade > 1){ red_shade = 1;}

                r *= red_shade;
                g *= red_shade;
                b *= red_shade;                    

                SDL_SetRenderDrawColor((*game)->renderer, r, g, b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint((*game)->renderer, x, y);

                pixel += 3;
            }
        }
    }else{
        if(screen_number == 3){
            for(int y = 0; y < BUFFER_HEIGHT; y++){
                for(int x = 0; x < BUFFER_WIDTH; x++){
                    r = escaped[pixel + 0]*shade;
                    g = escaped[pixel + 1]*shade;
                    b = escaped[pixel + 2]*shade;

                    SDL_SetRenderDrawColor((*game)->renderer, r, g, b, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawPoint((*game)->renderer, x, y);

                    pixel += 3;
                }
            }
        }else{
            if(screen_number == 4){
                for(int y = 0; y < BUFFER_HEIGHT; y++){
                    for(int x = 0; x < BUFFER_WIDTH; x++){
                        r = game_over[pixel + 0];
                        g = game_over[pixel + 1];
                        b = game_over[pixel + 2];

                        if(r != 255 || g != 0 || b != 255){
                            r *= shade;
                            g *= shade;
                            b *= shade;
                            SDL_SetRenderDrawColor((*game)->renderer, r, g, b, SDL_ALPHA_OPAQUE);
                            SDL_RenderDrawPoint((*game)->renderer, x, y);
                        }
                    
                        pixel += 3;
                    }
                }
            }else{
                if(screen_number == 5){
                    for(int y = 0; y < BUFFER_HEIGHT; y++){
                        for(int x = 0; x < BUFFER_WIDTH; x++){
                            r = pause_screen[pixel + 0]*shade;
                            g = pause_screen[pixel + 1]*shade;
                            b = pause_screen[pixel + 2]*shade;

                            SDL_SetRenderDrawColor((*game)->renderer, r, g, b, SDL_ALPHA_OPAQUE);
                            SDL_RenderDrawPoint((*game)->renderer, x, y);

                            pixel += 3;
                        }
                    }
                }else{
                    if(screen_number == 6){
                        for(int y = 0; y < BUFFER_HEIGHT; y++){
                            for(int x = 0; x < BUFFER_WIDTH; x++){
                                r = press[pixel + 0];
                                g = press[pixel + 1];
                                b = press[pixel + 2];

                                if(r != 255 || g != 0 || b != 255){
                                    r *= shade;
                                    g *= shade;
                                    b *= shade;
                                    SDL_SetRenderDrawColor((*game)->renderer, r, g, b, SDL_ALPHA_OPAQUE);
                                    SDL_RenderDrawPoint((*game)->renderer, x, y);
                                }
                            
                                pixel += 3;
                            }
                        }
                    }
                }
            }  
        }   
    }

}

void render_frame(SDL_Renderer *renderer){
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

int handle_event(Raycaster *rc, Game *game, bool *playing, int *channel){
    double oldDirX;
    double oldPlaneX;

    if(is_alive((*game)->player)){
        if (get_w((*game)->keys) == 1){
            //Prev X and Y
            int x, y;
            x = (int)((*rc)->player_pos_x);
            y = (int)((*rc)->player_pos_y);

            if (on_map((*game)->map, (int)((*rc)->player_pos_x + (*rc)->player_dir_x * MV_SPEED),(int)((*rc)->player_pos_y)) <= 0)
                (*rc)->player_pos_x += (*rc)->player_dir_x * MV_SPEED;
            if (on_map((*game)->map, (int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y + (*rc)->player_dir_y * MV_SPEED)) <= 0)
                (*rc)->player_pos_y += (*rc)->player_dir_y * MV_SPEED;

            //Enqueue
            if (x != (int)((*rc)->player_pos_x) || y != (int)((*rc)->player_pos_y)){
                update_element((*game)->element, (int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y));
                printf("%s\n", enqueue((*game)->queue, (*game)->element) ? "OK" : "ERRO");
            }
        }

        if (get_s((*game)->keys)){
            if (on_map((*game)->map,(int)((*rc)->player_pos_x - (*rc)->player_dir_x * MV_SPEED), (int)((*rc)->player_pos_y)) <= 0)
                (*rc)->player_pos_x -= (*rc)->player_dir_x * MV_SPEED;
            if (on_map((*game)->map,(int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y - (*rc)->player_dir_y * MV_SPEED)) <= 0)
                (*rc)->player_pos_y -= (*rc)->player_dir_y * MV_SPEED;
        }

        if (get_d((*game)->keys)){
            oldDirX = (*rc)->player_dir_x;
            (*rc)->player_dir_x = (*rc)->player_dir_x * cos(-ROT_SPEED) - (*rc)->player_dir_y * sin(-ROT_SPEED);
            (*rc)->player_dir_y = oldDirX * sin(-ROT_SPEED) + (*rc)->player_dir_y * cos(-ROT_SPEED);
            oldPlaneX = (*rc)->player_plane_x;
            (*rc)->player_plane_x = (*rc)->player_plane_x * cos(-ROT_SPEED) - (*rc)->player_plane_y * sin(-ROT_SPEED);
            (*rc)->player_plane_y = oldPlaneX * sin(-ROT_SPEED) + (*rc)->player_plane_y * cos(-ROT_SPEED);
        }

        if (get_a((*game)->keys)){
            oldDirX = (*rc)->player_dir_x;
            (*rc)->player_dir_x = (*rc)->player_dir_x * cos(ROT_SPEED) - (*rc)->player_dir_y * sin(ROT_SPEED);
            (*rc)->player_dir_y = oldDirX * sin(ROT_SPEED) + (*rc)->player_dir_y * cos(ROT_SPEED);
            oldPlaneX = (*rc)->player_plane_x;
            (*rc)->player_plane_x = (*rc)->player_plane_x * cos(ROT_SPEED) - (*rc)->player_plane_y * sin(ROT_SPEED);
            (*rc)->player_plane_y = oldPlaneX * sin(ROT_SPEED) + (*rc)->player_plane_y * cos(ROT_SPEED);
        }

        //Get item event
        if (on_map((*game)->map, (int)( (*rc)->player_pos_x + ( (*rc)->player_dir_x ) * MV_SPEED ), (int)( (*rc)->player_pos_y)) == 9){               
            play_chunk((*game)->pick_up_keys);
            get_item((*game)->player);
            clear_item((*game)->map);
        }
        if (on_map((*game)->map, (int)( (*rc)->player_pos_x ), (int)( (*rc)->player_pos_y + ( (*rc)->player_dir_y ) * MV_SPEED )) == 9){
            play_chunk((*game)->pick_up_keys);
            get_item((*game)->player);
            clear_item((*game)->map);
        }

        //Change map event
        if(get_e((*game)->keys)){
            if (on_map((*game)->map, (int)((*rc)->player_pos_x + (*rc)->player_dir_x * MV_SPEED),(int)((*rc)->player_pos_y)) != 0)
                change_map_event(game, on_map((*game)->map, (int)((*rc)->player_pos_x + (*rc)->player_dir_x * MV_SPEED),(int)((*rc)->player_pos_y)), playing, channel);
            if (on_map((*game)->map, (int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y + (*rc)->player_dir_y * MV_SPEED)) != 0)
                change_map_event(game, on_map((*game)->map, (int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y + (*rc)->player_dir_y * MV_SPEED)), playing, channel);
        }

        //Pause game
        if(((get_esc((*game)->keys) && get_state(game) != 3) || get_state(game) == 5)){
            change_state(game, 5);
            draw_screens(game, 5, 1, 0);
            SDL_RenderPresent((*game)->renderer);
            while(get_state(game) == 5){
                read_keys(&(*game)->keys);
                if(get_enter((*game)->keys)){
                    change_state(game, 2);
                }
                if(get_k((*game)->keys)){
                    restart_game(game);
                    change_state(game, 1);
                }
                if (read_keys(&(*game)->keys) != 0)
                    (*game)->quit = true; 
            }
        }
    }
        
    return (0);
}

void changing_map(Game *game, int room[][MAPHEIGHT], int x, int y){
    generate_map(&(*game)->map, room);
    (*game)->raycaster->player_pos_x = x;
    (*game)->raycaster->player_pos_y = y;
}

void change_map_event(Game *game, int door, bool *playing, int *channel){
    if(Mix_Playing(*channel) == 0){
        *playing = false;
    }

    switch (door){
    case 12:
        if(get_qtd_keys((*game)->player) >= 1){
            play_chunk((*game)->openning_door);
            changing_map(game, second_room, 19, 22);
            update_actual_room((*game)->map, 2);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }else{
            printf("Porta trancada!\n");
            if(!*playing){
                *channel = play_chunk((*game)->closed_door);
                *playing = true;
            }
        }
        break;
    case 21:
        if(get_qtd_keys((*game)->player) >= 1){
            play_chunk((*game)->openning_door);
            changing_map(game, main_room, 7, 11);
            update_actual_room((*game)->map, 1);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }
        break;
    case 13:
        if(get_qtd_keys((*game)->player) >= 2){
            play_chunk((*game)->openning_door);
            changing_map(game, third_room, 19, 2);
            update_actual_room((*game)->map, 3);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }else{
            printf("Porta trancada!\n");
            if(!*playing){
                *channel = play_chunk((*game)->closed_door);
                *playing = true;
            }
        }
        break;
    case 31:
        if(get_qtd_keys((*game)->player) >= 2){
            play_chunk((*game)->openning_door);
            changing_map(game, main_room, 7, 12);
            update_actual_room((*game)->map, 1);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }
        break;
    case 14:
        if(get_qtd_keys((*game)->player) >= 3){
            play_chunk((*game)->openning_door);
            changing_map(game, fourth_room, 4, 2);
            update_actual_room((*game)->map, 4);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }
        else{
            printf("Porta trancada!\n");
            if(!*playing){
                *channel = play_chunk((*game)->closed_door);
                *playing = true;
            }
        }
        break;
    case 41:
        if(get_qtd_keys((*game)->player) >= 3){
            play_chunk((*game)->openning_door);
            changing_map(game, main_room, 15, 12);
            update_actual_room((*game)->map, 1);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }
        break;
    case 15:
        if(get_qtd_keys((*game)->player) >= 4){
            play_chunk((*game)->openning_door);
            changing_map(game, fifth_room, 4, 22);
            update_actual_room((*game)->map, 5);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }else{
            printf("Porta trancada!\n");
            if(!*playing){
                *channel = play_chunk((*game)->closed_door);
                *playing = true;
            }
        }
        break;
    case 51:
        if(get_qtd_keys((*game)->player) >= 4){
            play_chunk((*game)->openning_door);
            changing_map(game, main_room, 15, 11);
            update_actual_room((*game)->map, 1);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }
        break;  
    case 16:
        if(get_qtd_keys((*game)->player) >= 5){
            play_chunk((*game)->openning_door);
            printf("Você escapou!\n");
            change_state(game, 3);
        }
        else{
            printf("Porta trancada!\n");
            if(!*playing){
                *channel = play_chunk((*game)->closed_door);
                *playing = true;
            }
        }
        break;
    case 17:
        if(get_qtd_keys((*game)->player) >= 5){
            play_chunk((*game)->openning_door);
            printf("Você escapou!\n");
            change_state(game, 3);
        }
        else{
            printf("Porta trancada!\n");
            if(!*playing){
                *channel = play_chunk((*game)->closed_door);
                *playing = true;
            }
        }
        break;
    }
}

void render_loop(Raycaster *rc, Game *game){
    
    //Frames
    Uint32 frameStart;
    int frameTime;

    //Timer
    int timer = 0;
    int timer_breathing = 0;

    //Shade
    double shade = 0;
    double red_shade = 0;

    //Dead
    int red = 0;

    //Start
    int start = 0;

    //Control audio flag
    bool playing_heavy_breathing = false;
    int channel_heavy_breathing = 0;

    bool playing_closed_door = false;
    int channel_closed_door = 0;
    
    while (!(*game)->quit){

        frameStart = SDL_GetTicks();
        timer += 1;
        timer_breathing += 1;

        //Game running
        if(get_state(game) == 2){
            start = 0;
            for (int x = 0; x < BUFFER_WIDTH; x++){
                calculating(rc, x);
                dda(rc, &(*game)->map);
                draw_texture(rc, x, (*game)->renderer, (*game)->map, red);
            }

            show_keys((*game)->font, (*game)->renderer, (*game)->player);
            handle_event(rc, game, &playing_closed_door, &channel_closed_door);

            //Monster events
            if(timer >= 30){
                m_chasing((*game)->queue, (*game)->monster, (*game)->player, (*game)->monster_walking);
                timer = 0;
            }

            //Control Audio flag
            if(Mix_Playing(channel_heavy_breathing) == 0){
                playing_heavy_breathing = false;
            }

            //Monster is close
            if(is_walking((*game)->monster)){
                if(get_size((*game)->queue) == 4 && !playing_heavy_breathing){
                    channel_heavy_breathing = play_chunk((*game)->heavy_breathing);
                    playing_heavy_breathing = true;
                }
            }

            //Dead
            if(!is_alive((*game)->player)){
                Mix_Pause(channel_heavy_breathing);
                if(shade > 1){ shade = 1;}
                draw_screens(game, 4, shade, red_shade);
                shade += 0.01;
                red += 2;
                if(red > 40){red = 40;}
                if(get_enter((*game)->keys)){
                    restart_game(game);
                }
                if(get_esc((*game)->keys)){
                    (*game)->quit = true;
                }
            }
            
        }else {
            //Home screen
            if(get_state(game) == 1){

                if(!start)
                    if(get_enter((*game)->keys) && start == 0){ shade = 0; start = 1;}
 
                if(shade > 1){ shade = 1;}
                if(red_shade > 1){ red_shade = 1;}
                draw_screens(game, 1, shade, red_shade);
                if(timer > 120 && !start){draw_screens(game, 6, 1, 0);}
                
                red_shade += 0.01;

                if(start){
                    shade += 0.01;
                    printf("%d\n", timer);
                    if(shade >=1 || get_movement_buttons((*game)->keys) == 1 ){
                        change_state(game, 2);
                        timer = 0;
                        shade = 0;
                    }
                }
                if(get_esc((*game)->keys)){
                    (*game)->quit = true;
                }
                
            }else{
                //Escape screen
                if(get_state(game) == 3){
                    if(shade > 1){ shade = 1;}
                    draw_screens(game, 3, shade, red_shade);
                    shade += 0.01;
                    if(get_enter((*game)->keys)){
                        restart_game(game);
                    }
                    if(get_esc((*game)->keys)){
                        (*game)->quit = true;
                    }
                }
            }
            
        }

        render_frame((*game)->renderer);

        if (read_keys(&(*game)->keys) != 0)
            (*game)->quit = true;     

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime)
            SDL_Delay(FRAME_DELAY - frameTime);
        
    }
}

void close_raycaster(Raycaster *rc){
    if(rc != NULL){
        free(*rc);
    }
}