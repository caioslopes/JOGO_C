#include "player.h"

struct player
{
    int pocket[SIZE_POCKET];
    int i_pocket;
};


Player init_player(){
    Player player;
    player = malloc(sizeof(struct player));
    player->i_pocket = -1;
    init_pocket(&player);
    return player;

}

void init_pocket(Player *player){
    if(pocket_is_empty(player)){
        for(int i = 0; i < SIZE_POCKET; i++){
            (*player)->pocket[i] = 0;
            printf("pocket[%d] = %d\n", i, (*player)->pocket[i]);
        }
    }
}

void get_item(Player *player, int item){
    if(!pocket_is_full(player)){
        if(!search_item(player, item)){
            (*player)->i_pocket += 1;
            (*player)->pocket[(*player)->i_pocket] = item;
            printf("Pegou o item: %d\n", (*player)->pocket[(*player)->i_pocket]);
        }
    }
}

bool search_item(Player *player, int item){
    bool find = false;
    if(!pocket_is_empty(player)){
        for(int i = 0; i <= (*player)->i_pocket && find == false; i++){
            if((*player)->pocket[i] == item){
                find = true;
                printf("Chave encontrada!\n");
            }
        }
    }
    return find;
}

bool pocket_is_full(Player *player){
   return (*player)->i_pocket == SIZE_POCKET-1;
}

bool pocket_is_empty(Player *player){
    return (*player)->i_pocket == -1;
}