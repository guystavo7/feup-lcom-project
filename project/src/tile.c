#include "../include/tile.h"
#include "../include/vec2d.h"
#include <stdlib.h>

struct Tile {
    const char* sprite;
    Vec2d pos;
};

Tile* tileNew(const char* sprite, int x, int y) {
  Tile* tile = malloc(sizeof(Tile));
  tile->sprite = sprite;
  tile->pos.x = x;
  tile->pos.y = y;
}

void tileFree(Tile* tile) {
  free(tile);
}
