#ifndef MAP_H_
#define MAP_H_

#include "utils.h"

// Map
#define MAPWIDTH 24
#define MAPHEIGHT 24

extern int main_room[MAPWIDTH][MAPHEIGHT];
extern int second_room[MAPWIDTH][MAPHEIGHT];
extern int third_room[MAPWIDTH][MAPHEIGHT];
extern int fourth_room[MAPWIDTH][MAPHEIGHT];
extern int fifth_room[MAPWIDTH][MAPHEIGHT];

typedef struct map* Map;

void init_map(Map *map);

void update_actual_room(Map map, int room);

int get_actual_room(Map map);

void generate_map(Map *map, int room[][MAPHEIGHT]);

void clear_item(Map map);

int on_map(Map map, int x, int y);

double on_map_double(Map map, int x, int y);

void close_map(Map *map);

#endif