#ifndef TILE
#define TILE

#include "boxCollider.h"
#include <stdbool.h>
#include "../../lab5/terminal.h"
#include "../../lab5/spriteXMP.h"

typedef struct Tile {
    SpriteXPM* sprite;
    int id;
    Vec2d pos;
    Vec2d size;
    BoxCollider* collider;
} Tile;

Tile* tileNew(char** sprite, int name, int x, int y, int tileSize);

void tileFree(Tile* tile);

void tileUpdate(Tile* tile, Terminal* terminal);

BoxCollider* tileBoxCollider(Tile* tile);

#endif
