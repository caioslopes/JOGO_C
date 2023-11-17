#ifndef RENDER_H_
#define RENDER_H_

#include <math.h>
#include "app.h"

// Colors
#define WHITE 0xFFFFFF
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define BLACK 0x000000
#define PURPLE 0x00FFFF

// Texture
#define TILE 64

// Map
#define MAPWIDTH 24
#define MAPHEIGHT 24

extern int worldMap[MAPWIDTH][MAPHEIGHT];
extern int brick_texture[];
extern int BIGSQUARES[];
extern int all[];
extern int sprites[];

// Inicial values
#define INIT_P_POS_X 22
#define INIT_P_POS_Y 12
#define INIT_P_DIR_X -1
#define INIT_P_DIR_Y 0
#define INIT_P_PLANE_X 0
#define INIT_P_PLANE_Y 0.66
#define MV_SPEED 0.199
#define ROT_SPEED 0.1745
#define AMB_LIGHT 7

typedef struct s_raycaster{
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
} t_raycaster;

// Functions Render
void starting_values(t_raycaster *rc);
void calculating(t_raycaster *rc, int w);
void dda(t_raycaster *rc);
int calc_wall_height(t_raycaster *rc);
void draw_point(t_raycaster *rc, int x);
void render_frame();
int handle_events(t_raycaster *rc);
void render(t_raycaster *rc);

// Functions Colors
SDL_Color convert_color(int hexa_value);
SDL_Color select_wall_color(int map_x, int map_y);
SDL_Color apply_night_effect(SDL_Color color, double distance);

#endif