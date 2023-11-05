#include "raycaster.h"
#include "maps.h"
#include "player.h"

typedef struct ButtonKeys* keys;

void init_keys(keys* key);

int read_keys(keys key);

void move_player(Map map, keys key, t_raycaster* rc, Player player);