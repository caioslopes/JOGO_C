#ifndef FONTS_H_
#define FONTS_H_

#include "utils.h"
#include "game.h"

#define SIZE_FONT 48

void show_keys(TTF_Font *font, SDL_Renderer *renderer, Player player);

void close_font(TTF_Font *font);

#endif