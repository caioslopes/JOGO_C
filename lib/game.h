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
#include "fonts.h"

/******************
* APPLICATION
*******************/

// Sreen
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define BUFFER_WIDTH 426
#define BUFFER_HEIGHT 240

// Frame rate
#define FRAME_RATE 60
#define FRAME_DELAY 1000 / FRAME_RATE

// Fundamental information about the application
typedef struct game* Game;

void init_game(Game *game);

void game_running(Game game);

void change_state(Game *game);

bool get_state(Game *game);

void quit_aplication(Game *game);

/******************
* RAYCASTER
*******************/

// Texture
#define TILE 64
extern int textures[];
extern int home_screen[];
extern int home_screen2[];
extern int game_over[];

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
int choosing_texture(Raycaster *rc, Map map);
void draw_texture(Raycaster *rc, int x, SDL_Renderer *renderer, Map map, int red);
void draw_home(Game *game, int screen_number, double shade);
void render_frame(SDL_Renderer  *renderer);
int handle_event(Raycaster *rc, Game *game);
void change_map_event(Game *game, int door);
void changing_map(Game *game, int room[][MAPHEIGHT], int x, int y);
void render_loop(Raycaster *rc, Game *game);
void close_raycaster(Raycaster *rc);

#endif