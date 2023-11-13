#include "default.h"
#include "raycaster.h"
#include "buttons.h"
#include "queue.h"

#define SIZE_POCKET 4

typedef struct player* Player;

Player init_player();

void init_pocket(Player *player);

void set_is_alive(Player *player, bool status);

bool get_is_alive(Player player);

void get_item(Player *player, int item, Map map, Mix_Chunk *getkeys);

bool search_item(Player *player, int item);

bool pocket_is_full(Player *player);

bool pocket_is_empty(Player *player);

bool is_collided(Map map, t_raycaster *rc, int ray);