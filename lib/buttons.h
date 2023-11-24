#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "utils.h"

typedef struct keys* ButtonKeys;

void init_buttons(ButtonKeys *key);

int read_keys(ButtonKeys *key); 

int get_w(ButtonKeys key);

int get_a(ButtonKeys key);

int get_d(ButtonKeys key);

int get_s(ButtonKeys key);

int get_e(ButtonKeys key);

int get_k(ButtonKeys key);

int get_esc(ButtonKeys key);

int get_enter(ButtonKeys key);

int get_movement_buttons(ButtonKeys key);

void close_buttons(ButtonKeys *keys);

#endif