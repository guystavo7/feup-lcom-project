#include "../include/tile.h"
#include "../include/vec2d.h"
#include <stdlib.h>
#include <string.h>

Tile* tileNew(const char* sprite, const char* name, int x, int y, int tileSize) {
  Tile* tile = malloc(sizeof(Tile));
  tile->sprite = sprite;
  tile->name = name;
  tile->pos.x = x;
  tile->pos.y = y;
  tile->size.x = tileSize;
  tile->size.y = tileSize;

  if (strcmp(name, "wall") == 0) {
      Vec2d offset = {0, 0};
      tile->collider = boxColliderNew(tile->pos, offset, tile->size, 0);
  } else {
      tile->collider = NULL;
  }

  return tile;
}

void tileFree(Tile* tile) {
  if (tile->collider != NULL) {
      boxColliderFree(tile->collider);
  }
  free(tile);
}

BoxCollider* tileBoxCollider(Tile* tile) {
  return tile->collider;
}
