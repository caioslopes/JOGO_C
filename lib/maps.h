#include "raycaster.h"

// Map
#define mapWidth 24
#define mapHeight 24

extern int a[mapWidth][mapHeight];
extern int b[mapWidth][mapHeight];
extern int c[mapWidth][mapHeight];
extern int d[mapWidth][mapHeight];

typedef struct map* Map;

Map make_map(int room[][mapHeight]);

void generate_map(Map* map, int room[][mapHeight]);

void change_map(Map* map);

int get_value_of(Map map, int x, int y);