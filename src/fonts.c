#include "../lib/fonts.h"

void pointer_text(Player player, char retorno[]);

void show_keys(TTF_Font *font, SDL_Renderer *renderer, Player player){
    SDL_Rect Message_rect; //create a rect
    SDL_Color White = {.r = 255, .g = 255, .b = 255};
    Message_rect.x = SCREEN_WIDTH - (SIZE_FONT * 78);  //controls the rect's x coordinate 
    Message_rect.y = SIZE_FONT - (SIZE_FONT/2); // controls the rect's y coordinte
    Message_rect.w = SIZE_FONT * 6; // controls the width of the rect
    Message_rect.h = SIZE_FONT; // controls the height of the rect*/

    char msg[15];
    pointer_text(player, msg);

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, msg, White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void pointer_text(Player player, char retorno[]){
    char answer[15] = "Chaves: ";
    answer[8] = get_qtd_keys_string(player);
    strcat(answer, " | 5");
    strcpy(retorno, answer);
}

void close_font(TTF_Font *font){
    TTF_CloseFont(font);
    TTF_Quit();
}