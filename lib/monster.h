#include "raycaster.h"
#include "queue.h"
#include "player.h"

typedef struct monster* Monster;

Monster init_monster();

void m_walk(Queue *queue);

bool is_walking(Monster monster);

void set_walking(Monster *monster, bool status);

void kill_player(Player *player);