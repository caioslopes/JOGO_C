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
    if (!pocket_is_full(player))
    {
        if (item > 82 && item < 87)
        {
            if (!search_item(player, item))
            {
                (*player)->i_pocket += 1;
                (*player)->pocket[(*player)->i_pocket] = item;
                printf("Pegou o item: %d\n", (*player)->pocket[(*player)->i_pocket]);
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

void move_player(Map map, ButtonKeys key, t_raycaster *rc, Player player, Queue queue)
{
    if (get_is_alive(player))
    {

        double oldDirX;
        double oldPlaneX;

        if (get_w(key) == 1)
        {

            /* Get Item */
            if (get_value_of(map, (int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) > 82 && get_value_of(map, (int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) < 87)
            {
                get_item(&player, get_value_of(map, (int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)), map);
            }
            if (get_value_of(map, (int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)) > 82 && get_value_of(map, (int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)) < 87)
            {
                get_item(&player, get_value_of(map, (int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)), map);
            }

            /* Change Map*/
            if (get_value_of(map, (int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) != 0)
            {
                change_map(get_value_of(map, (int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)), &map, rc, key, player);
            }
            if (get_value_of(map, (int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)) != 0)
            {
                change_map(get_value_of(map, (int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)), &map, rc, key, player);
            }

            int x, y;
            x = (int)(rc->player_pos_x);
            y = (int)(rc->player_pos_y);

            if (get_value_of(map, (int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) == 0)
            {                                                    // verifica se aonde o player está é uma posição de valor 0 (sem parede) no mapa
                rc->player_pos_x += rc->player_dir_x * MV_SPEED; // incrementa a posição do player no eixo X
            }
            if (get_value_of(map, (int)(rc->player_pos_x), (int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)) == 0)
            {                                                    // verifica se aonde o player está é uma posição de valor 0 (sem parede) no mapa
                rc->player_pos_y += rc->player_dir_y * MV_SPEED; // incrementa a posição do player no eixo Y
            }

            if (x != (int)(rc->player_pos_x) || y != (int)(rc->player_pos_y))
            {
                printf("%s\n", enqueue(queue, 1) ? "OK" : "ERRO");
            }
        }

        if (get_a(key) == 1)
        {
            oldDirX = rc->player_dir_x;
            rc->player_dir_x = rc->player_dir_x * cos(ROT_SPEED) - rc->player_dir_y * sin(ROT_SPEED);
            rc->player_dir_y = oldDirX * sin(ROT_SPEED) + rc->player_dir_y * cos(ROT_SPEED);
            oldPlaneX = rc->player_plane_x;
            rc->player_plane_x = rc->player_plane_x * cos(ROT_SPEED) - rc->player_plane_y * sin(ROT_SPEED);
            rc->player_plane_y = oldPlaneX * sin(ROT_SPEED) + rc->player_plane_y * cos(ROT_SPEED);
        }

        if (get_s(key) == 1)
        {
            if (get_value_of(map, (int)(rc->player_pos_x - rc->player_dir_x * MV_SPEED), (int)(rc->player_pos_y)) == 0)
            {
                rc->player_pos_x -= rc->player_dir_x * MV_SPEED;
            }
            if (get_value_of(map, (int)(rc->player_pos_x), (int)(rc->player_pos_y - rc->player_dir_y * MV_SPEED)) == 0)
            {
                rc->player_pos_y -= rc->player_dir_y * MV_SPEED;
            }
        }

        if (get_d(key) == 1)
        {
            oldDirX = rc->player_dir_x;
            rc->player_dir_x = rc->player_dir_x * cos(-ROT_SPEED) - rc->player_dir_y * sin(-ROT_SPEED);
            rc->player_dir_y = oldDirX * sin(-ROT_SPEED) + rc->player_dir_y * cos(-ROT_SPEED);
            oldPlaneX = rc->player_plane_x;
            rc->player_plane_x = rc->player_plane_x * cos(-ROT_SPEED) - rc->player_plane_y * sin(-ROT_SPEED);
            rc->player_plane_y = oldPlaneX * sin(-ROT_SPEED) + rc->player_plane_y * cos(-ROT_SPEED);
        }
    }
}

void change_map(int door, Map *map, t_raycaster *rc, ButtonKeys key, Player player)
{
    switch (door)
    {
    case 12:
        if (get_e(key) == 1)
        {
            generate_map(map, first_room);
            rc->player_pos_x = 19;
            rc->player_pos_y = 22;
        }
        break;
    case 21:
        if (get_e(key) == 1)
        {
            generate_map(map, main_room);
            rc->player_pos_x = 7;
            rc->player_pos_y = 11;
        }
        break;
    case 13:
        if (get_e(key) == 1)
        {
            if (search_item(&player, 83))
            {
                generate_map(map, second_room);
                rc->player_pos_x = 19;
                rc->player_pos_y = 2;
            }
        }
        break;
    case 31:
        if (get_e(key) == 1)
        {
            if (search_item(&player, 83))
            {
                generate_map(map, main_room);
                rc->player_pos_x = 7;
                rc->player_pos_y = 12;
            }
        }
        break;
    case 14:
        if (get_e(key) == 1)
        {
            if (search_item(&player, 84))
            {
                generate_map(map, third_room);
                rc->player_pos_x = 4;
                rc->player_pos_y = 2;
            }
        }
        break;
    case 41:
        if (get_e(key) == 1)
        {
            if (search_item(&player, 84))
            {
                generate_map(map, main_room);
                rc->player_pos_x = 15;
                rc->player_pos_y = 12;
            }
        }
        break;
    case 15:
        if (get_e(key) == 1)
        {
            if (search_item(&player, 85))
            {
                generate_map(map, fourth_room);
                rc->player_pos_x = 4;
                rc->player_pos_y = 22;
            }
        }
        break;
    case 51:
        if (get_e(key) == 1)
        {
            if (search_item(&player, 85))
            {
                generate_map(map, main_room);
                rc->player_pos_x = 15;
                rc->player_pos_y = 11;
            }
        }
        break;
    case 16:
        if (get_e(key) == 1)
        {
            if (search_item(&player, 86))
            {
                generate_map(map, final_room);
                rc->player_pos_x = 22;
                rc->player_pos_y = 11;
            }
        }
        break;
    case 61:
        if (get_e(key) == 1)
        {
            if (search_item(&player, 86))
            {
                generate_map(map, main_room);
                rc->player_pos_x = 2;
                rc->player_pos_y = 11;
            }
        }
        break;
    }
}