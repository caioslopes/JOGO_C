#ifndef MONSTER_H_
#define MONSTER_H_

#include "utils.h"
#include "queue.h"
#include "player.h"
#include "sounds.h"

#define START_WALKING 10
#define JUMP 3

typedef struct monster* Monster;

void init_monster(Monster *monster);

void m_chasing(Queue queue, Monster monster, Player player, Mix_Chunk *monster_walking);

void m_walk(Queue queue, Monster monster);

void m_jump(Queue queue, Monster monster, Player player, int size_jump);

bool is_walking(Monster monster);

void set_walking(Monster monster, bool status);

void kill_player(Player player);

void close_monster(Monster *monster);

#endif