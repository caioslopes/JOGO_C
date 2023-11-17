#include "render.h"

SDL_Surface *window_surface;
SDL_Texture *screen_texture;

int main(){
    t_raycaster rc;
    init_aplication();
    starting_values(&rc);
    render(&rc);
}

void starting_values(t_raycaster *rc){
    rc->player_pos_x = INIT_P_POS_X;
    rc->player_pos_y = INIT_P_POS_Y;
    rc->player_dir_x = INIT_P_DIR_X;
    rc->player_dir_y = INIT_P_DIR_Y;
    rc->player_plane_x = INIT_P_PLANE_X;
    rc->player_plane_y = INIT_P_PLANE_Y;
}

void calculating(t_raycaster *rc, int w){
    double camera_x;

    camera_x = 2 * w / (double)(SCREEN_WIDTH)-1;

    rc->ray_dir_x = rc->player_dir_x + rc->player_plane_x * camera_x;
    rc->ray_dir_y = rc->player_dir_y + rc->player_plane_y * camera_x;

    rc->map_x = (int)(rc->player_pos_x);
    rc->map_y = (int)(rc->player_pos_y);

    rc->delta_dist_x = sqrt(1 + pow(rc->ray_dir_y, 2) / pow(rc->ray_dir_x, 2));
    rc->delta_dist_y = sqrt(1 + pow(rc->ray_dir_x, 2) / pow(rc->ray_dir_y, 2));

    if (rc->ray_dir_x < 0){
        rc->step_x = -1;
        rc->side_dist_x = (rc->player_pos_x - rc->map_x) * rc->delta_dist_x;
    }else{
        rc->step_x = 1;
        rc->side_dist_x = (rc->map_x + 1.0 - rc->player_pos_x) * rc->delta_dist_x;
    }

    if (rc->ray_dir_y < 0){
        rc->step_y = -1;
        rc->side_dist_y = (rc->player_pos_y - rc->map_y) * rc->delta_dist_y;
    }else{
        rc->step_y = 1;
        rc->side_dist_y = (rc->map_y + 1.0 - rc->player_pos_y) * rc->delta_dist_y;
    }
}

void dda(t_raycaster *rc){
    int hit;
    hit = 0;

    while (hit == 0){
        if (rc->side_dist_x < rc->side_dist_y){
            rc->side_dist_x += rc->delta_dist_x;
            rc->map_x += rc->step_x;
            rc->side = 0;
        }else{
            rc->side_dist_y += rc->delta_dist_y;
            rc->map_y += rc->step_y;
            rc->side = 1;
        }

        if (worldMap[rc->map_x][rc->map_y] > 0){
            hit = 1;
        }
    }
}

int calc_wall_height(t_raycaster *rc){
    int line_height;

    if (rc->side == 0){
        rc->perp_wall_dist = (rc->map_x - rc->player_pos_x + (1 - rc->step_x) / 2) / rc->ray_dir_x;
    }else{
        rc->perp_wall_dist = (rc->map_y - rc->player_pos_y + (1 - rc->step_y) / 2) / rc->ray_dir_y;
    }

    line_height = (int)(SCREEN_HEIGHT / rc->perp_wall_dist);
    rc->draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;

    if (rc->draw_start < 0){
        rc->draw_start = 0;
    }

    rc->draw_end = line_height / 2 + SCREEN_HEIGHT / 2;

    if (rc->draw_end >= SCREEN_HEIGHT){
        rc->draw_end = SCREEN_HEIGHT - 1;
    }

    return line_height;
}

void draw_point(t_raycaster *rc, int x){

    int line_height = calc_wall_height(rc);

    //calculate value of wallX
    double wallX; //where exactly the wall was hit

    if (rc->side == 0){
        wallX = rc->player_pos_y + rc->perp_wall_dist * rc->ray_dir_y;
    }else{
        wallX = rc->player_pos_x + rc->perp_wall_dist * rc->ray_dir_x;
    }

    wallX -= floor((wallX));

    //x coordinate on the texture
    int texX = (int)(wallX * (double)(TILE));

    if(rc->side == 0 && rc->ray_dir_x > 0){
        texX = TILE - texX - 1;  
    }
    if(rc->side == 1 && rc->ray_dir_y < 0){
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
    for(int y = rc->draw_start; y < rc->draw_end; y++){
        
        int pixel = ((int)(ty)*32+texX) * 3 + (32*32*3);

        int r = all[pixel+0];
        int g = all[pixel+1];
        int b = all[pixel+2];

        if(rc->side == 1){
            r -= 30;
            g -= 30;
            b -= 30;
        }

        r -= (int)(rc->perp_wall_dist * 9);
        g -= (int)(rc->perp_wall_dist * 9);
        b -= (int)(rc->perp_wall_dist * 9);

        if (r < 0){ r = 0; }
        if (g < 0){ g = 0; }
        if (b < 0){ b = 0; }

        ty += ty_step;

        SDL_SetRenderDrawColor(application.renderer, r, g, b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(application.renderer, x, y);
    }
}

void render_frame()
{
    SDL_RenderPresent(application.renderer);
    SDL_SetRenderDrawColor(application.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(application.renderer);
}

int handle_events(t_raycaster *rc)
{
    SDL_Event event;
    double oldDirX;
    double oldPlaneX;

    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT)
            return (-1);
        if (event.type == SDL_KEYDOWN){
            if (event.key.keysym.sym == SDLK_UP){
                if (worldMap[(int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED)][(int)(rc->player_pos_y)] == 0)
                    rc->player_pos_x += rc->player_dir_x * MV_SPEED;
                if (worldMap[(int)(rc->player_pos_x)][(int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)] == 0)
                    rc->player_pos_y += rc->player_dir_y * MV_SPEED;
            }
            if (event.key.keysym.sym == SDLK_DOWN){
                if (worldMap[(int)(rc->player_pos_x - rc->player_dir_x * MV_SPEED)][(int)(rc->player_pos_y)] == 0)
                    rc->player_pos_x -= rc->player_dir_x * MV_SPEED;
                if (worldMap[(int)(rc->player_pos_x)][(int)(rc->player_pos_y - rc->player_dir_y * MV_SPEED)] == 0)
                    rc->player_pos_y -= rc->player_dir_y * MV_SPEED;
            }
            if (event.key.keysym.sym == SDLK_RIGHT){
                oldDirX = rc->player_dir_x;
                rc->player_dir_x = rc->player_dir_x * cos(-ROT_SPEED) - rc->player_dir_y * sin(-ROT_SPEED);
                rc->player_dir_y = oldDirX * sin(-ROT_SPEED) + rc->player_dir_y * cos(-ROT_SPEED);
                oldPlaneX = rc->player_plane_x;
                rc->player_plane_x = rc->player_plane_x * cos(-ROT_SPEED) - rc->player_plane_y * sin(-ROT_SPEED);
                rc->player_plane_y = oldPlaneX * sin(-ROT_SPEED) + rc->player_plane_y * cos(-ROT_SPEED);
            }
            if (event.key.keysym.sym == SDLK_LEFT){
                oldDirX = rc->player_dir_x;
                rc->player_dir_x = rc->player_dir_x * cos(ROT_SPEED) - rc->player_dir_y * sin(ROT_SPEED);
                rc->player_dir_y = oldDirX * sin(ROT_SPEED) + rc->player_dir_y * cos(ROT_SPEED);
                oldPlaneX = rc->player_plane_x;
                rc->player_plane_x = rc->player_plane_x * cos(ROT_SPEED) - rc->player_plane_y * sin(ROT_SPEED);
                rc->player_plane_y = oldPlaneX * sin(ROT_SPEED) + rc->player_plane_y * cos(ROT_SPEED);
            }
        }
    }
    return (0);
}

void render(t_raycaster *rc){
    while (!application.quit){
        for (int x = 0; x < SCREEN_WIDTH; x++){
            calculating(rc, x);
            dda(rc);
            draw_point(rc, x);
        }

        render_frame();

        if (handle_events(rc) != 0){
            application.quit = true;
        }
    }
}