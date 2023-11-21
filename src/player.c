#include "../lib/player.h"

struct player{
    bool is_alive;
    int qtd_keys;
};

void init_player(Player *player){
    Player p;
    p = malloc(sizeof(struct player));

    p->qtd_keys = 0;
    p->is_alive = true;

    if(player != NULL){
        *player = p;
    }else{
        printf("Error, & is NULL - Player");
    }
}

void set_is_alive(Player player, bool status){
    player->is_alive = status;
}

bool is_alive(Player player){
    return player->is_alive;
}

void get_item(Player player){
    if(get_qtd_keys(player) <= MAX_KEYS){
        player->qtd_keys += 1;
    }
}

int get_qtd_keys(Player player){
    return player->qtd_keys;
}

char get_qtd_keys_string(Player player){
    char answer = '0';

    switch (get_qtd_keys(player))
    {
    case 1:
        answer = '1';
        break;
    case 2:
        answer = '2';
        break;
    case 3:
        answer = '3';
        break;
    case 4:
        answer = '4';
        break;
    case 5:
        answer = '5';
        break;
    
    }

    return answer;
}

void close_player(Player *player){
    if(player != NULL){
        free(*player);
    }
}