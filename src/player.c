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

void move_player(Map map, ButtonKeys key, t_raycaster *rc, Player player){
    double oldDirX;
    double oldPlaneX;

    if(get_w(key) == 1){

        /* Get Item */
        if(get_value_of(map,(int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) != 0){
             for(int i = 83; i <= 86; i++){
                if(get_value_of(map,(int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) == i){
                    get_item(&player, i);
                    clear_item(&map, i);
                }
            }
        }
        if(get_value_of(map,(int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)) != 0){
            for(int i = 83; i <= 86; i++){
                if(get_value_of(map,(int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)) == i){
                    get_item(&player, i);
                    clear_item(&map, i);
                }
            }
        }

        /* Change Map*/
        if(get_value_of(map,(int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) != 0){
            switch (get_value_of(map,(int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)))
            {
            case 12:
                    generate_map(&map, first_room);
                break;
            case 21:
                    generate_map(&map, main_room);
                break;
            case 13:
                if(search_item(&player, 83))
                    generate_map(&map, second_room);
                break;
            case 31:
                if(search_item(&player, 83))
                    generate_map(&map, main_room);
                break;
            case 14:
                if(search_item(&player, 84))
                    generate_map(&map, third_room);
                break;
            case 41:
                if(search_item(&player, 84))
                    generate_map(&map, main_room);
                break;
            case 15:
                if(search_item(&player, 85))
                    generate_map(&map, fourth_room);
                break;
            case 51:
                if(search_item(&player, 85))
                    generate_map(&map, main_room);
                break;
            case 16:
                if(search_item(&player, 86))
                    generate_map(&map, final_room);
                break;
            case 61:
                if(search_item(&player, 86))
                    generate_map(&map, main_room);
                break;
            }
        }
        if(get_value_of(map,(int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)) != 0){
            switch (get_value_of(map,(int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)))
            {
            case 12:
                    generate_map(&map, first_room);
                break;
            case 21:
                    generate_map(&map, main_room);
                break;
            case 13:
                if(search_item(&player, 83))
                    generate_map(&map, second_room);
                break;
            case 31:
                if(search_item(&player, 83))
                    generate_map(&map, main_room);
                break;
            case 14:
                if(search_item(&player, 84))
                    generate_map(&map, third_room);
                break;
            case 41:
                if(search_item(&player, 84))
                    generate_map(&map, main_room);
                break;
            case 15:
                if(search_item(&player, 85))
                    generate_map(&map, fourth_room);
                break;
            case 51:
                if(search_item(&player, 85))
                    generate_map(&map, main_room);
                break;
            case 16:
                if(search_item(&player, 86))
                    generate_map(&map, final_room);
                break;
            case 61:
                if(search_item(&player, 86))
                    generate_map(&map, main_room);
                break;
            }
        }

        if(get_value_of(map,(int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) == 0){ // verifica se aonde o player está é uma posição de valor 0 (sem parede) no mapa  
            rc->player_pos_x += rc->player_dir_x * MV_SPEED; // incrementa a posição do player no eixo X 
        }
        if(get_value_of(map,(int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)) == 0) { // verifica se aonde o player está é uma posição de valor 0 (sem parede) no mapa
            rc->player_pos_y += rc->player_dir_y * MV_SPEED;  // incrementa a posição do player no eixo Y
        }
    }

    if(get_a(key) == 1){
        oldDirX = rc->player_dir_x;
        rc->player_dir_x = rc->player_dir_x * cos(ROT_SPEED) - rc->player_dir_y * sin(ROT_SPEED);
        rc->player_dir_y = oldDirX * sin(ROT_SPEED) + rc->player_dir_y * cos(ROT_SPEED);
        oldPlaneX = rc->player_plane_x;
        rc->player_plane_x = rc->player_plane_x * cos(ROT_SPEED) - rc->player_plane_y * sin(ROT_SPEED);
        rc->player_plane_y = oldPlaneX * sin(ROT_SPEED) + rc->player_plane_y * cos(ROT_SPEED);
    }

    if(get_s(key) == 1){
        if(get_value_of(map, (int)(rc->player_pos_x - rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) == 0) {
            rc->player_pos_x -= rc->player_dir_x * MV_SPEED;
        }
        if(get_value_of(map, (int)(rc->player_pos_x), (int)(rc->player_pos_y - rc->player_dir_y * MV_SPEED)) == 0) {
            rc->player_pos_y -= rc->player_dir_y * MV_SPEED;
        }
    }

    if(get_d(key) == 1){
        oldDirX = rc->player_dir_x;
        rc->player_dir_x = rc->player_dir_x * cos(-ROT_SPEED) - rc->player_dir_y * sin(-ROT_SPEED);
        rc->player_dir_y = oldDirX * sin(-ROT_SPEED) + rc->player_dir_y * cos(-ROT_SPEED);
        oldPlaneX = rc->player_plane_x;
        rc->player_plane_x = rc->player_plane_x * cos(-ROT_SPEED) - rc->player_plane_y * sin(-ROT_SPEED);
        rc->player_plane_y = oldPlaneX * sin(-ROT_SPEED) + rc->player_plane_y * cos(-ROT_SPEED);
    }
}