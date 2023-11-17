#include "app.h"
#include "render.h"

t_app application;

void init_aplication(){
    application.quit = false;
    application.playing = true;
    application.menu = false;

    printf("Starting...\n");

    SDL_Init(SDL_INIT_EVERYTHING);

    application.window = SDL_CreateWindow("Não olhe para trás", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    application.renderer = SDL_CreateRenderer(application.window, -1, SDL_RENDERER_PRESENTVSYNC);
}

void change_state(){
    if(application.playing){
        application.playing = false;
        application.menu = true;
    }else{
        application.menu = false;
        application.playing = true;
    }
}

void quit_aplication(){
    SDL_Quit();
}