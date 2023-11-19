#include "../lib/fonts.h"

void show_keys(TTF_Font *font, SDL_Renderer *renderer, Player player){
    SDL_Rect Message_rect; //create a rect
    SDL_Color White = {255, 255, 255};
    Message_rect.x = 0;  //controls the rect's x coordinate 
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 20; // controls the width of the rect
    Message_rect.h = 10; // controls the height of the rect*/

    char msg[10] = "Chaves: ";
    msg[8] += get_qtd_keys_string(player);
    msg[9] += '\0';

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, msg, White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void close_font(TTF_Font *font){
    TTF_CloseFont(font);
    TTF_Quit();
}