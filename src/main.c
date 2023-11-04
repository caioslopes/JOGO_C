#include "raycaster.h"
#include "buttons.h"
#include "maps.h"

int init(t_sdl *sdl, t_raycaster *rc);
void initial_calc(t_raycaster *rc, int x);
void perform_dda(t_raycaster *rc, Map map);
void calc_wall_height(t_raycaster *rc);
void draw_vert_line(Map map, t_sdl *sdl, t_raycaster *rc, int x, keys key);
void render_frame(t_sdl *sdl);
void raycaster(t_sdl *sdl, t_raycaster *rc);

int main(){
  t_sdl sdl;
  t_raycaster rc;

  if (init(&sdl, &rc) != 0)
    return (-1);
  raycaster(&sdl, &rc);
  if (sdl.renderer)
    SDL_DestroyRenderer(sdl.renderer);
  if (sdl.window)
    SDL_DestroyWindow(sdl.window);
  SDL_Quit();
  return (0);
}

int init(t_sdl *sdl, t_raycaster *rc){
  sdl->window = NULL;
  sdl->renderer = NULL;
  rc->player_pos_x = INIT_P_POS_X;
  rc->player_pos_y = INIT_P_POS_Y;
  rc->player_dir_x = INIT_P_DIR_X;
  rc->player_dir_y = INIT_P_DIR_Y;
  rc->player_plane_x = INIT_P_PLANE_X;
  rc->player_plane_y = INIT_P_PLANE_Y;
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    fprintf(stderr,"SDL initialization failed (%s)\n",SDL_GetError());
    return (-1);
  }
  if (SDL_CreateWindowAndRenderer(WIN_X, WIN_Y, 0, &sdl->window, &sdl->renderer) != 0){
    fprintf(stderr,"Window creation failed (%s)\n",SDL_GetError());
    return (-1);
  }
  return (0);
}

void initial_calc(t_raycaster *rc, int x){
  double      camera_x;

  camera_x = 2 * x / (double)(WIN_X) - 1;
  rc->ray_dir_x = rc->player_dir_x + rc->player_plane_x * camera_x;
  rc->ray_dir_y = rc->player_dir_y + rc->player_plane_y * camera_x;
  rc->map_x = (int)(rc->player_pos_x);
  rc->map_y = (int)(rc->player_pos_y);
  rc->delta_dist_x = sqrt(1 + pow(rc->ray_dir_y, 2) / pow(rc->ray_dir_x, 2));
  rc->delta_dist_y = sqrt(1 + pow(rc->ray_dir_x, 2) / pow(rc->ray_dir_y, 2));
  if (rc->ray_dir_x < 0){
    rc->step_x = -1;
    rc->side_dist_x = (rc->player_pos_x - rc->map_x) * rc->delta_dist_x;
  }
  else{
    rc->step_x = 1;
    rc->side_dist_x = (rc->map_x + 1.0 - rc->player_pos_x) * rc->delta_dist_x;
  }
  if (rc->ray_dir_y < 0){
    rc->step_y = -1;
    rc->side_dist_y = (rc->player_pos_y - rc->map_y) * rc->delta_dist_y;
  }
  else{
    rc->step_y = 1;
    rc->side_dist_y = (rc->map_y + 1.0 - rc->player_pos_y) * rc->delta_dist_y;
  }
}

void perform_dda(t_raycaster *rc, Map map){
  int hit;

  hit = 0;
  while (hit == 0){
    if (rc->side_dist_x < rc->side_dist_y){
      rc->side_dist_x += rc->delta_dist_x;
      rc->map_x += rc->step_x;
      rc->side = 0;
    }
    else{
      rc->side_dist_y += rc->delta_dist_y;
      rc->map_y += rc->step_y;
      rc->side = 1;
    }
    if (get_value_of(map, rc->map_x, rc->map_y) > 0)
      hit = 1;
  }
}

void calc_wall_height(t_raycaster *rc){
  int line_height;

  if (rc->side == 0)
    rc->perp_wall_dist = (rc->map_x - rc->player_pos_x + (1 - rc->step_x) / 2) / rc->ray_dir_x;
  else
    rc->perp_wall_dist = (rc->map_y - rc->player_pos_y + (1 - rc->step_y) / 2) / rc->ray_dir_y;
  line_height = (int)(WIN_Y / rc->perp_wall_dist);
  rc->draw_start = -line_height / 2 + WIN_Y / 2;
  if (rc->draw_start < 0)
    rc->draw_start = 0;
  rc->draw_end = line_height / 2 + WIN_Y / 2;
  if (rc->draw_end >= WIN_Y)
    rc->draw_end = WIN_Y - 1;
}

void draw_vert_line(Map map, t_sdl *sdl, t_raycaster *rc, int x, keys key){
  SDL_Color color;
  
  color = apply_night_effect(select_wall_color(map,rc->map_x, rc->map_y), rc->perp_wall_dist);

  if (rc->side == 1){
    color.r /= 2;
    color.g /= 2;
    color.b /= 2;
  }

  SDL_SetRenderDrawColor(sdl->renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(sdl->renderer, x, rc->draw_start, x, rc->draw_end);

  move_player(map, key, rc);

}

void render_frame(t_sdl *sdl){
  SDL_RenderPresent(sdl->renderer);
  SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(sdl->renderer);
}


void raycaster(t_sdl *sdl, t_raycaster *rc){
  SDL_bool done;

  Map map;
  map = make_map(a);

  /* Keys */
  keys key;
  init_keys(&key);

  done = SDL_FALSE;

  /* FPS */
  int fps = 60;
  int frameDelay = 1000 / fps;
  Uint32 frameStart;
  int frameTime;
  
  while(!done){
    frameStart = SDL_GetTicks();

    for(int x = 0; x < WIN_X; x++){
      initial_calc(rc, x);
      perform_dda(rc, map);
      calc_wall_height(rc);
      draw_vert_line(map, sdl, rc, x, key);
    }

    render_frame(sdl);

    if (read_keys(key) != 0)
      done = SDL_TRUE;

    frameTime = SDL_GetTicks() - frameStart;
    if(frameDelay > frameTime){
      SDL_Delay(frameDelay - frameTime);
    }

  }
}