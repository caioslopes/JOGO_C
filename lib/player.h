#ifndef PLAYER_H_
#define PLAYER_H_

#include "utils.h"

#define MAX_KEYS 5

typedef struct player* Player;

void init_player(Player *player);

void set_is_alive(Player player, bool status);

bool is_alive(Player player);

void get_item(Player player);

int get_qtd_keys(Player player);

void close_player(Player *player);

#endif