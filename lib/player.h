#include "raycaster.h"

#define SIZE_POCKET 4

typedef struct player* Player;

Player init_player();

void init_pocket(Player *player);

void get_item(Player *player, int item);

bool search_item(Player *player, int item);

bool pocket_is_full(Player *player);

bool pocket_is_empty(Player *player);