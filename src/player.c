#include "player.h"

struct player
{
    bool is_alive;
    int pocket[SIZE_POCKET];
    int i_pocket;
};

Player init_player()
{
    Player player;
    player = malloc(sizeof(struct player));
    player->i_pocket = -1;
    player->is_alive = true;
    init_pocket(&player);
    return player;
}

void set_is_alive(Player *player, bool status)
{
    (*player)->is_alive = status;
}

bool get_is_alive(Player player)
{
    return player->is_alive;
}

void init_pocket(Player *player)
{
    if (pocket_is_empty(player))
    {
        for (int i = 0; i < SIZE_POCKET; i++)
        {
            (*player)->pocket[i] = 0;
            printf("pocket[%d] = %d\n", i, (*player)->pocket[i]);
        }
    }
}

void get_item(Player *player, int item, Map map)
{
    Mix_Chunk *getKeys = Mix_LoadWAV("../sounds/keys.wav");
    if (!pocket_is_full(player))
    {
        if (item > 82 && item < 87)
        {
            if (!search_item(player, item))
            {
                (*player)->i_pocket += 1;
                (*player)->pocket[(*player)->i_pocket] = item;
                printf("Pegou o item: %d\n", (*player)->pocket[(*player)->i_pocket]);
                Mix_PlayChannel(-1, getKeys, 0);
                switch (item)
                {
                case 83:
                    clear_item(&map, first_room, 83);
                    break;
                case 84:
                    clear_item(&map, second_room, 84);
                    break;
                case 85:
                    clear_item(&map, third_room, 85);
                    break;
                case 86:
                    clear_item(&map, fourth_room, 86);
                    break;
                }
            }
        }
    }
}

bool search_item(Player *player, int item)
{
    bool find = false;
    if (!pocket_is_empty(player))
    {
        for (int i = 0; i <= (*player)->i_pocket && find == false; i++)
        {
            if ((*player)->pocket[i] == item)
            {
                find = true;
            }
        }
    }
    return find;
}

bool pocket_is_full(Player *player)
{
    return (*player)->i_pocket == SIZE_POCKET - 1;
}

bool pocket_is_empty(Player *player)
{
    return (*player)->i_pocket == -1;
}

bool is_collided(Map map, t_raycaster *rc, int ray)
{
    bool collide = false;

    if (rc->draw_end == WIN_Y - 1 && (ray > (WIN_X / 5 * 2) || ray < WIN_X/5 * 4))
    {
        if (get_value_of(map, (int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) > 0)
        {
            if (get_value_of(map, (int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) < 5)
            {
                collide = true;
            }
        }
        if (get_value_of(map, (int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)) > 0)
        {
            if (get_value_of(map, (int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)) < 5)
            {
                collide = true;
            }
        }
    }

    return collide;
}