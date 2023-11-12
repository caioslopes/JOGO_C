#include "default.h"
#include "maps.h"

#define AMB_LIGHT 2

// Colors
#define WHITE 0xFFFFFF
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define BLACK 0x000000
#define PURPLE 0x800080
#define YELLOW 0xffff00
#define GRAY 0x8d8d8d
#define MARRON 0x800000

SDL_Color convert_color(int hexa_value);  
SDL_Color select_wall_color(Map map, int map_x, int map_y);
SDL_Color apply_night_effect(SDL_Color color, double distance);