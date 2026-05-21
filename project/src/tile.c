#include "../include/tile.h"
#include "../include/utils/vec2d.h"
#include <stdlib.h>

Tile* tileNew(char** sprite, int id, int x, int y, int tileSize) {
    Tile* tile = malloc(sizeof(Tile));

    if (sprite != NULL) {
        tile->sprite = newSpriteXPM((xpm_map_t)sprite, XPM_8_8_8, 0, 0);
    } else {
        tile->sprite = NULL;
    }

    tile->id = id;
    tile->pos.x = x;
    tile->pos.y = y;
    tile->size.x = tileSize;
    tile->size.y = tileSize;

    // Disable collisions for all map tiles
    tile->collider = NULL;

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

void tileUpdate(Tile* tile, Terminal* terminal) {
    if (tile->sprite != NULL) {
        drawSpriteXPM(tile->sprite, terminal, tile->pos.x, tile->pos.y, false);
    }
}
