#include "raycaster.h"
#include "maps.h"

SDL_Color convert_color(int hexa_value){
  SDL_Color   color;

  color.r = ((hexa_value >> 16) & 0xFF);
  color.g = ((hexa_value >> 8) & 0xFF);
  color.b = ((hexa_value) & 0xFF);
  return (color); 
}

SDL_Color select_wall_color(Map map, int map_x, int map_y){
  SDL_Color color;

  if (get_value_of(map, map_x, map_y) == 1){
    color = convert_color(RED);
  }
  else if (get_value_of(map, map_x, map_y) == 2){
    color = convert_color(GREEN);
  }
  else if (get_value_of(map, map_x, map_y) == 3){
    color = convert_color(PURPLE);
  }
  else if (get_value_of(map, map_x, map_y) == 4){
    color = convert_color(WHITE);
  }
  else if (get_value_of(map, map_x, map_y) == 12 || get_value_of(map, map_x, map_y) == 21 ){
    color = convert_color(GRAY1);
  }
  else if (get_value_of(map, map_x, map_y) == 13 || get_value_of(map, map_x, map_y) == 31){
    color = convert_color(GRAY2);
  }
  else if (get_value_of(map, map_x, map_y) == 14 || get_value_of(map, map_x, map_y) == 41){
    color = convert_color(GRAY3);
  }
  else if (get_value_of(map, map_x, map_y) == 82 || get_value_of(map, map_x, map_y) == 83 || get_value_of(map, map_x, map_y) == 84){
    color = convert_color(MARRON);
  }
  else {
    color = convert_color(BLACK);
  }
  return (color);
}

SDL_Color apply_night_effect(SDL_Color color, double distance){
  
  if (color.r/distance * AMB_LIGHT <= color.r){
    color.r = color.r/distance * AMB_LIGHT;
  }
  if (color.g/distance * AMB_LIGHT <= color.g){
    color.g = color.g/distance * AMB_LIGHT;
  }
  if (color.b/distance * AMB_LIGHT <= color.b){
    color.b = color.b/distance * AMB_LIGHT;
  }
  
  return (color);
}
