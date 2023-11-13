#ifndef RAYCASTER_H_
#define RAYCASTER_H_

// System include
#include "default.h"

// Initial values
#define WIN_X 1280
#define WIN_Y 720
#define INIT_P_POS_X 22
#define INIT_P_POS_Y 12
#define INIT_P_DIR_X -1
#define INIT_P_DIR_Y 0
#define INIT_P_PLANE_X 0
#define INIT_P_PLANE_Y 0.66
#define MV_SPEED 0.00005
#define ROT_SPEED 0.00001745

#define texWidth 64
#define texHeight 64

// Structures
typedef struct s_sdl{
  SDL_Window *window;
  SDL_Renderer *renderer;
} t_sdl;

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

// Local includes
#include "maps.h"
#include "colors.h"
#include "player.h"
#include "monster.h"

int init(t_sdl *sdl, t_raycaster *rc);
void initial_calc(t_raycaster *rc, int x);
void perform_dda(t_raycaster *rc, Map map);
void calc_wall_height(t_raycaster *rc);
void draw_vert_line(Map map, t_sdl *sdl, t_raycaster *rc, int x);
void render_frame(t_sdl *sdl);
void raycaster(t_sdl *sdl, t_raycaster *rc, Map *map, Player *player, ButtonKeys *key, Queue *queue, Monster *monster);
void move_player(Map map, ButtonKeys key, t_raycaster* rc, Player player, Queue queue, Mix_Chunk *doorAudio, Mix_Chunk *getkeys);
void change_map(int door, Map *map, t_raycaster *rc, ButtonKeys key, Player player, Queue queue, Mix_Chunk *doorAudio);

#endif