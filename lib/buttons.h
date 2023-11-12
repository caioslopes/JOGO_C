#include "raycaster.h"

typedef struct keys* ButtonKeys;

ButtonKeys init_keys();

int read_keys(ButtonKeys key);

int get_w(ButtonKeys key);

int get_a(ButtonKeys key);

int get_d(ButtonKeys key);

int get_s(ButtonKeys key);

int get_e(ButtonKeys key);

int get_esc(ButtonKeys key);