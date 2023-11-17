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

void m_walk(Queue *queue, Monster monster){
    if(is_walking(monster)){
        dequeue(*queue);
        printf("Monstro andando!\n");
    }
}

void m_jump(Queue *queue, int size_jump){
    for(int i = 0; i < size_jump && !is_empty(*queue); i++){
        dequeue(*queue);
    }
    printf("Monstro pulou!\n");
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