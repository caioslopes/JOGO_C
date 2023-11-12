#include "default.h"

// Map
#define mapWidth 24
#define mapHeight 24

extern int main_room[mapWidth][mapHeight];
extern int first_room[mapWidth][mapHeight];
extern int second_room[mapWidth][mapHeight];
extern int third_room[mapWidth][mapHeight];
extern int fourth_room[mapWidth][mapHeight];
extern int final_room[mapWidth][mapHeight];


typedef struct map* Map;

Map init_map();

void generate_map(Map *map, int room[][mapHeight]);

void clear_item(Map *map, int room[][mapHeight], int item);

int get_value_of(Map map, int x, int y);