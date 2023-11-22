#ifndef FONTS_H_
#define FONTS_H_

#include "utils.h"
#include "game.h"

#define SIZE_FONT 12

void show_keys(TTF_Font *font, SDL_Renderer *renderer, Player player);

void pointer_text(Player player, char retorno[]);

void close_font(TTF_Font *font);

#endif