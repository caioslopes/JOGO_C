#include "../lib/monster.h"

struct monster{
    bool walking;
};

void init_monster(Monster *monster){
    Monster m;
    m = malloc(sizeof(struct monster));
    m->walking = false;

    if(monster != NULL){
        *monster = m;
    }
}

void m_chasing(Queue queue, Monster monster, Player player, Mix_Chunk *monster_walking){

    if(get_size(queue) == 10){
        set_walking(monster, true);
    }

    if(is_walking(monster)){
        if(is_empty(queue)){
            kill_player(player);
            set_walking(monster, false);
        }else{
            m_walk(queue, monster);
            play_chunk(monster_walking);
        }
    }
}

void m_walk(Queue queue, Monster monster){
    if(is_walking(monster)){
        dequeue(queue);
        printf("Monstro andando!\n");
    }
}

void m_jump(Queue queue, Monster monster, Player player, int size_jump){
    if(is_walking(monster)){
        if(is_empty(queue)){
            kill_player(player);
        }else{
            for(int i = 0; i < size_jump && !is_empty(queue); i++){
                dequeue(queue);
            }
        }
        printf("Monstro pulou!\n");
    }
}

bool is_walking(Monster monster){
    return monster->walking;
}

void set_walking(Monster monster, bool status){
    monster->walking = status;
}

void kill_player(Player player){
    set_is_alive(player, false);
    printf("Player Morto...\n");
}

void close_monster(Monster *monster){
    if(monster != NULL){
        free(*monster);
    }
}