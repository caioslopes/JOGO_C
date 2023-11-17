#ifndef ENGINE_H_
#define ENGINE_H_

// Lib
#include "utils.h"
#include "buttons.h"

/******************
* Application
*******************/

// Sreen
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Frame rate
#define FRAME_RATE 30
#define FRAME_TIME 1000 / FRAME_RATE

// Fundamental information about the application
typedef struct game* Game;

void init_game(Game *game);

/* void change_state();

void quit_aplication(); */

/******************
* RAYCASTER
*******************/

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

typedef struct raycaster* Raycaster;

// Functions Render
void init_raycaster(Raycaster *rc);
void starting_values(Raycaster *rc);
void calculating(Raycaster *rc, int w);
void dda(Raycaster *rc);
int calc_wall_height(Raycaster *rc);
void draw_point(Raycaster *rc, int x, SDL_Renderer *renderer);
void render_frame(SDL_Renderer  *renderer);
int moviment(Raycaster *rc, ButtonKeys keys);
void render_loop(Raycaster *rc, Game *game);

// Functions Colors
SDL_Color convert_color(int hexa_value);
SDL_Color select_wall_color(int map_x, int map_y);
SDL_Color apply_night_effect(SDL_Color color, double distance);

#endif