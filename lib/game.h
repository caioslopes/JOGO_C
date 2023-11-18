#ifndef ENGINE_H_
#define ENGINE_H_

// Lib
#include "utils.h"
#include "buttons.h"
#include "map.h"
#include "player.h"
#include "queue.h"
#include "monster.h"
#include "sounds.h"

/******************
* APPLICATION
*******************/

// Sreen
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 360

// Frame rate
#define FRAME_RATE 30
#define FRAME_TIME 1000 / FRAME_RATE

// Fundamental information about the application
typedef struct game* Game;

void init_game(Game *game);

void game_running(Game game);

/* void change_state(); */

void quit_aplication(Game *game);

/******************
* RAYCASTER
*******************/

// Texture
#define TILE 64
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
#define MV_SPEED 599 / 10000.0
#define ROT_SPEED 3745 / 100000.0
#define AMB_LIGHT 7

typedef struct raycaster* Raycaster;

// Functions Render
void init_raycaster(Raycaster *rc);
void starting_values(Raycaster *rc);
void calculating(Raycaster *rc, int w);
void dda(Raycaster *rc, Map *map);
int calc_wall_height(Raycaster *rc);
void draw_point(Raycaster *rc, int x, SDL_Renderer *renderer);
void render_frame(SDL_Renderer  *renderer);
int moviment_event(Raycaster *rc, Game *game);
void change_map_event(Game *game, int door);
void changing_map(Game *game, int room[][MAPHEIGHT], int x, int y);
void render_loop(Raycaster *rc, Game *game);
void close_raycaster(Raycaster *rc);

#endif