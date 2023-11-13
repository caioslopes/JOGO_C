#include "default.h"
#include "queue.h"
#include "player.h"

#define START_WALKING 10
#define JUMP 4

typedef struct monster* Monster;

Monster init_monster();

void m_walk(Queue *queue, Monster monster);

void m_jump(Queue *queue, int size_jump);

bool is_walking(Monster monster);

void set_walking(Monster *monster, bool status);

void kill_player(Player *player);