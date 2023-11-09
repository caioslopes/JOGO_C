#include "monster.h"

struct monster
{
    bool walking;
};

Monster init_monster(){
    Monster m;
    m = malloc(sizeof(struct monster));
    m->walking = false;
    return m;
}

void m_walk(Queue *queue){
    dequeue(*queue);
}

bool is_walking(Monster monster){
    return monster->walking;
}

void set_walking(Monster *monster, bool status){
    (*monster)->walking = status;
}

void kill_player(Player *player){
    set_is_alive(player, false);
}