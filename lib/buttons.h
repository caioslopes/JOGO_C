#include "raycaster.h"

typedef struct ButtonKeys* keys;

void init_keys(keys* key);

int read_keys(keys key);

void move_player(Map map, keys key, t_raycaster* rc);