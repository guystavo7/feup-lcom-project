#ifndef TILE
#define TILE

#include "boxCollider.h"
#include <stdbool.h>

typedef struct Tile Tile;

Tile* tileNew(const char* sprite, int x, int y);

void tileFree(Tile* tile);

#endif
