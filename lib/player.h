#include "default.h"
#include "raycaster.h"
#include "buttons.h"
#include "queue.h"

#define SIZE_POCKET 4

typedef struct player* Player;

Player init_player();

void init_pocket(Player *player);

void set_is_alive(Player *player, bool status);

void get_item(Player *player, int item, Map map);

bool search_item(Player *player, int item);

bool pocket_is_full(Player *player);

bool pocket_is_empty(Player *player);

void move_player(Map map, ButtonKeys key, t_raycaster* rc, Player player, Queue queue);

void change_map(int door, Map *map, t_raycaster *rc, ButtonKeys key, Player player);