#ifndef TILE
#define TILE

#include "boxCollider.h"
#include <stdbool.h>

typedef struct Tile {
    const char* sprite;
    const char* name;
    Vec2d pos;
    Vec2d size;
    BoxCollider* collider;
} Tile;

Tile* tileNew(const char* sprite, const char* name, int x, int y, int tileSize);

void tileFree(Tile* tile);

BoxCollider* tileBoxCollider(Tile* tile);

#endif
