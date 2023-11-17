#ifndef APP_H_
#define APP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

// Sreen
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Fundamental information about the application
typedef struct s_app{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool playing;
    bool menu;
    bool quit;
}t_app;

// Declarations
extern t_app application;

extern SDL_Surface *window_surface;
extern SDL_Texture *screen_texture;

void init_aplication();

void change_state();

void quit_aplication();

#endif