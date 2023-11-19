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

    //Sounds
    Mix_Music *sound_track;
    Mix_Chunk *monster_walking;
    Mix_Chunk *openning_door;
    Mix_Chunk *closed_door;
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

    g->window = SDL_CreateWindow("Não olhe para trás", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    g->renderer = SDL_CreateRenderer(g->window, -1, SDL_RENDERER_ACCELERATED);

    // Game options
    g->quit = false;
    g->playing = true;
    g->menu = false;

    //Sounds
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    g->sound_track = Mix_LoadMUS("assets/sounds/sound_track.mp3");
    g->monster_walking = Mix_LoadWAV("assets/sounds/monster_walking.wav");
    /* g->closed_door = ; */
    g->openning_door = Mix_LoadWAV("assets/sounds/openning_door.wav");
    g->pick_up_keys = Mix_LoadWAV("assets/sounds/pick_up_keys.wav");

    //Fonts
    g->font = TTF_OpenFont("assets/fonts/VCR_OSD_MONO_1.001.ttf", SIZE_FONT);

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

void quit_aplication(Game *game){
    //Rendering
    SDL_DestroyWindow((*game)->window);
    SDL_DestroyRenderer((*game)->renderer);

    //Sounds
    Mix_FreeMusic((*game)->sound_track);
    Mix_FreeChunk((*game)->monster_walking);
    Mix_FreeChunk((*game)->openning_door);
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

int choosing_texture(Raycaster *rc, Map map){
    //number of the texture that will be loaded
    int texNum = on_map(map, (*rc)->map_x, (*rc)->map_y);

    //rendering doors
    if(texNum > 11 && texNum < 17 || texNum == 21 || texNum == 31 || texNum == 41 || texNum == 51){
        texNum = 3;
    }

    //rendering keys
    /* if(texNum == 9){
        //define number of keys
    } */

    return texNum;
}

void draw_texture(Raycaster *rc, int x, SDL_Renderer *renderer, Map map){

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

    int texNum = choosing_texture(rc, map);
    float ty = ty_off * ty_step;
    if(texNum == 9){
        ty += 1;
    }else{
        ty += texNum == 1 ? 1 : 32 * texNum; //choosing textures
    }

    texX =  (int)((float)texX * (32.0/(float)TILE));
    for(int y = (*rc)->draw_start; y < (*rc)->draw_end; y++){
        
        int pixel = ((int)(ty)*32+texX) * 3;
        int r, g, b;

        if(texNum != 9){
            r = all[pixel+0];
            g = all[pixel+1];
            b = all[pixel+2];
        }else{
            r = sp[pixel+0];
            g = sp[pixel+1];
            b = sp[pixel+2];
        }

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

void render_frame(SDL_Renderer *renderer){
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

int moviment_event(Raycaster *rc, Game *game){
    double oldDirX;
    double oldPlaneX;

    if(is_alive((*game)->player)){
        if (get_w((*game)->keys) == 1){
            //Prev X and Y
            int x, y;
            x = (int)((*rc)->player_pos_x);
            y = (int)((*rc)->player_pos_y);

            if (on_map((*game)->map, (int)((*rc)->player_pos_x + (*rc)->player_dir_x * MV_SPEED),(int)((*rc)->player_pos_y)) == 0)
                (*rc)->player_pos_x += (*rc)->player_dir_x * MV_SPEED;
            if (on_map((*game)->map, (int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y + (*rc)->player_dir_y * MV_SPEED)) == 0)
                (*rc)->player_pos_y += (*rc)->player_dir_y * MV_SPEED;

            //Enqueue
            if (x != (int)((*rc)->player_pos_x) || y != (int)((*rc)->player_pos_y)){
                update_element((*game)->element, (int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y));
                printf("%s\n", enqueue((*game)->queue, (*game)->element) ? "OK" : "ERRO");
            }

            //Get item event
            if (on_map((*game)->map, (int)((*rc)->player_pos_x + (*rc)->player_dir_x * MV_SPEED),(int)((*rc)->player_pos_y)) == 9){                
                play_chunk((*game)->pick_up_keys);
                get_item((*game)->player);
                clear_item((*game)->map);
            }
            if (on_map((*game)->map, (int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y + (*rc)->player_dir_y * MV_SPEED)) == 9){
                play_chunk((*game)->pick_up_keys);
                get_item((*game)->player);
                clear_item((*game)->map);
            }
                
            //Change map event
            if(get_e((*game)->keys)){
                if (on_map((*game)->map, (int)((*rc)->player_pos_x + (*rc)->player_dir_x * MV_SPEED),(int)((*rc)->player_pos_y)) != 0)
                    change_map_event(game, on_map((*game)->map, (int)((*rc)->player_pos_x + (*rc)->player_dir_x * MV_SPEED),(int)((*rc)->player_pos_y)));
                if (on_map((*game)->map, (int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y + (*rc)->player_dir_y * MV_SPEED)) != 0)
                    change_map_event(game, on_map((*game)->map, (int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y + (*rc)->player_dir_y * MV_SPEED)));
            }
            
        }
        if (get_s((*game)->keys)){
            if (on_map((*game)->map,(int)((*rc)->player_pos_x - (*rc)->player_dir_x * MV_SPEED), (int)((*rc)->player_pos_y)) == 0)
                (*rc)->player_pos_x -= (*rc)->player_dir_x * MV_SPEED;
            if (on_map((*game)->map,(int)((*rc)->player_pos_x), (int)((*rc)->player_pos_y - (*rc)->player_dir_y * MV_SPEED)) == 0)
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
    }
        
    return (0);
}

void changing_map(Game *game, int room[][MAPHEIGHT], int x, int y){
    generate_map(&(*game)->map, room);
    (*game)->raycaster->player_pos_x = x;
    (*game)->raycaster->player_pos_y = y;
}

void change_map_event(Game *game, int door){
    switch (door){
    case 12:
        if(get_qtd_keys((*game)->player) >= 1){
            play_chunk((*game)->openning_door);
            changing_map(game, second_room, 19, 22);
            update_actual_room((*game)->map, 2);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }else{
            printf("Porta trancada!\n");
        }
        break;
    case 21:
        if(get_qtd_keys((*game)->player) >= 1){
            play_chunk((*game)->openning_door);
            changing_map(game, main_room, 7, 11);
            update_actual_room((*game)->map, 1);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }else{
            printf("Porta trancada!\n");
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
        }
        break;
    case 31:
        if(get_qtd_keys((*game)->player) >= 2){
            play_chunk((*game)->openning_door);
            changing_map(game, main_room, 7, 12);
            update_actual_room((*game)->map, 1);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }else{
            printf("Porta trancada!\n");
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
        }
        break;
    case 41:
        if(get_qtd_keys((*game)->player) >= 3){
            play_chunk((*game)->openning_door);
            changing_map(game, main_room, 15, 12);
            update_actual_room((*game)->map, 1);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }else{
            printf("Porta trancada!\n");
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
        }
        break;
    case 51:
        if(get_qtd_keys((*game)->player) >= 4){
            play_chunk((*game)->openning_door);
            changing_map(game, main_room, 15, 11);
            update_actual_room((*game)->map, 1);
            m_jump((*game)->queue, (*game)->monster, (*game)->player, JUMP);
        }
        else{
            printf("Porta trancada!\n");
        }
        break;  
    case 16:
        if(get_qtd_keys((*game)->player) >= 5){
            play_chunk((*game)->openning_door);
            printf("Você escapou!\n");
        }
        else{
            printf("Porta trancada!\n");
        }
        break;
    }
}

void render_loop(Raycaster *rc, Game *game){
    
    // FPS
    int fps = 60;
    int frameDelay = 1000 / fps;
    Uint32 frameStart;
    int frameTime;

    //Timer
    int timer = 0;
    
    while (!(*game)->quit){

        frameStart = SDL_GetTicks();
        timer += 1;

        for (int x = 0; x < SCREEN_WIDTH; x++){
            calculating(rc, x);
            dda(rc, &(*game)->map);
            draw_texture(rc, x, (*game)->renderer, (*game)->map);
        }

        render_frame((*game)->renderer);
        
        moviment_event(rc, game);

        show_keys((*game)->font, (*game)->renderer, (*game)->player);

        //Monster events
        if(timer >= 60){
            m_chasing((*game)->queue, (*game)->monster, (*game)->player, (*game)->monster_walking);
            timer = 0;
        }

        if (read_keys(&(*game)->keys) != 0)
            (*game)->quit = true;

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
}

void close_raycaster(Raycaster *rc){
    if(rc != NULL){
        free(*rc);
    }
}