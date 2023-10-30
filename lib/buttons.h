#include "raycaster.h"

typedef struct ButtonKeys* keys;

void initKeys(keys* key);

int leTecla(keys key);

void movimenta(keys key, t_raycaster* rc);